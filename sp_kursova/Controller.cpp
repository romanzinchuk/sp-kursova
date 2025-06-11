#include "stdafx.h"
#include "Controller.h"
#include "Core/Parser/TokenParser.h"
#include "Core/Backus/BackusRuleStorage.h"
#include "Core/Generator/Generator.h"
#include "Core/Generator/GeneratorUtils.h"

#include "SimpleTokens.h"

#include "Tokens/Common/Program.h"
#include "Tokens/Common/Vars.h"

#include "Rules/IdentRule/IdentRule.h"
#include "Rules/VarsBlokRule/VarsBlokRule.h"
#include "Rules/EquationRule/EquationRule.h"
#include "Rules/ReadRule/ReadRule.h"
#include "Rules/WriteRule/WriteRule.h"
#include "Rules/AssignmentRule/AssignmentRule.h"

#include "Rules/Operators/LoopInterrupt/LoopBreak.h"
#include "Rules/Operators/LoopInterrupt/LoopContinue.h"

void Controller::regItem(std::shared_ptr<IToken> token, std::shared_ptr<IBackusRule> rule, ItemType type, int priority) const
{
    using enum ItemType;

    if ((type & Token) == Token)
        TokenParser::Instance()->regToken(token, ((type & Operation) == Operation) ? TokenParser::NoPriority : priority);

    if ((type & Rule) == Rule)
        BackusRuleStorage::Instance()->regRule(rule);

    auto tokenType = token->type();

    if ((type & Operand) == Operand)
        GeneratorUtils::Instance()->RegisterOperand(tokenType);

    if ((type & Operation) == Operation)
    {
        if (priority == TokenParser::NoPriority)
            throw std::runtime_error("Controller::RegItem: Operation " + token->type() +  " priority is not set");

        GeneratorUtils::Instance()->RegisterOperation(tokenType, priority);
    }

    if ((type & Lsqr) == Lsqr)
        GeneratorUtils::Instance()->RegisterLSquare(tokenType);

    if ((type & Rsqr) == Rsqr)
        GeneratorUtils::Instance()->RegisterRSquare(tokenType);

    if ((type & EquationEnd) == EquationEnd)
        GeneratorUtils::Instance()->RegisterEquationEnd(tokenType);

    if ((type & LBracket) == LBracket)
        GeneratorUtils::Instance()->RegisterLBraket(tokenType);

    if ((type & RBracket) == RBracket)
        GeneratorUtils::Instance()->RegisterRBraket(tokenType);

	const std::string defBreak = "_DEFBRK";
	GeneratorUtils::Instance()->RegisterDefaultBreak(defBreak);
}

void Controller::init()
{
    m_topRule = MakeTopRule(Instance());

    Generator::Instance()->setDetails(context()->Details());
}

void Controller::regUnchangedTextToken(std::shared_ptr<IToken> target, std::shared_ptr<IToken> lBorder, std::shared_ptr<IToken> rBorder) const
{
    TokenParser::Instance()->regUnchangedTextToken(target, lBorder, rBorder);
}

void Controller::regOperatorRule(const RuleMaker& rule, bool isNeedSemicolon)
{
    auto ruleName = rule(Instance())->type();

    if (ruleName.empty())
        throw std::runtime_error("Controller::RegOperatorRule: Rule name is empty");

    if (m_operatorRuleNames.contains(ruleName) || m_operatorRuleWithSemicolonNames.contains(ruleName))
        throw std::runtime_error(std::format("Controller::RegOperatorRule: Rule with name {} already registered", ruleName));

    if (isNeedSemicolon)
        m_operatorRuleWithSemicolonNames.insert(ruleName);
    else
        m_operatorRuleNames.insert(ruleName);
}

std::shared_ptr<IBackusRule> Controller::addRule(const std::string& name, const std::list<BackusRuleItem>& items) const
{
    auto rule = BackusRule::MakeRule(name, items);

    BackusRuleStorage::Instance()->regRule(rule);

    return rule;
}

BackusRulePtr Controller::topRule()
{
    if (!m_topRule)
        throw(std::runtime_error("Controller is not inited"));

    return m_topRule;
}

BackusRulePtr Controller::MakeTopRule(std::shared_ptr<Controller> controller) const
{
    using enum ItemType;
    auto ctx = controller->context();
    controller->regItem<Program>();
    controller->regItem<Vars>();
	controller->regItem<Start>(TokenAndRule | EquationEnd);
	controller->regItem<End>();
	controller->regItem<ProgStart>(TokenAndRule | EquationEnd);
	controller->regItem<ProgEnd>();

    controller->regItem<Comma>();
    controller->regItem<Colon>();
    controller->regItem<Semicolon>(TokenAndRule | EquationEnd);
    controller->regItem<LSquare>(TokenAndRule | Lsqr);
    controller->regItem<RSquare>(TokenAndRule | Rsqr);

    controller->regItem<LBraket>(TokenAndRule | LBracket);
    controller->regItem<RBraket>(TokenAndRule | RBracket);


    controller->regItem<LoopBreak>();
    controller->regItem<LoopContinue>();

    auto identRule = MakeIdentRule(controller);
    auto varsBlok = MakeVarsBlokRule(controller);
    auto equation = MakeEquationRule(controller);
    auto read = MakeReadRule(controller);
    auto write = MakeWriteRule(controller);
    auto assingmentRule = MakeAssignmentRule(controller);
    
    auto operatorWithSemicolonTypes = std::vector<std::variant<std::string, Symbols>>{ read->type(), write->type(), assingmentRule->type() ,LoopBreak::Type(), LoopContinue::Type()};
    operatorWithSemicolonTypes.insert(operatorWithSemicolonTypes.end(), m_operatorRuleWithSemicolonNames.begin(), m_operatorRuleWithSemicolonNames.end());

    auto operatorsWithSemicolon = controller->addRule("OperatorsWithSemicolon", {
        BackusRuleItem({ operatorWithSemicolonTypes }, OnlyOne),
        BackusRuleItem({ Symbols::Semicolon }, OnlyOne)
        });

    auto operatorTypes = std::vector<std::variant<std::string, Symbols>>{ m_operatorRuleNames.begin(), m_operatorRuleNames.end() };
    auto operators = controller->addRule("Operators", {
        BackusRuleItem({ operatorTypes }, OnlyOne)
        });

    auto operatorsRule = controller->addRule("OperatorsRule", {
        BackusRuleItem({ operators->type(), operatorsWithSemicolon->type()}, Optional | OneOrMore),
        });

    auto codeBlok = controller->addRule("CodeBlok", {
        BackusRuleItem({     Start::Type()}, OnlyOne),
        BackusRuleItem({ operators->type(), operatorsWithSemicolon->type()}, Optional | OneOrMore),
        BackusRuleItem({       End::Type()}, OnlyOne)
        });
    auto topRule = controller->addRule("TopRule", {
        BackusRuleItem({    Program::Type()}, OnlyOne),
        BackusRuleItem({  ctx->SingleIdentRuleName()}, OnlyOne),
        BackusRuleItem({ Symbols::Semicolon}, OnlyOne),
        BackusRuleItem({       Vars::Type()}, OnlyOne),
        BackusRuleItem({   varsBlok->type()}, OnlyOne),
        BackusRuleItem({     ProgStart::Type()}, OnlyOne),
        BackusRuleItem({ operators->type(), operatorsWithSemicolon->type()}, Optional | OneOrMore),
        BackusRuleItem({       ProgEnd::Type()}, OnlyOne)
        });

    return topRule;
}