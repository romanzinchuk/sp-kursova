#pragma once
#include "stdafx.h"
#include "Utils/singleton.hpp"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRule.h"
#include "Core/Generator/GeneratorItemBase.h"
#include "Core/Generator/GeneratorDetails.h"

#include "Symbols.h"

using BackusRulePtr = std::shared_ptr<IBackusRule>;
using BackusRuleList = std::list<BackusRulePtr>;
using BackusRuleListIt = BackusRuleList::iterator;
using RuleMaker = std::function<BackusRulePtr(std::shared_ptr<Controller>)>;

class Context
{
public:
    std::string SingleIdentRuleName() const { return "SingleIdentRule"; }
    std::string IdentRuleName() const { return "IdentRule"; }
    std::string EquationRuleName() const { return "Equation"; }
    std::string OperatorsRuleName() const { return "OperatorsRule"; }
    std::string AssignmentRuleName() const { return "AssignmentRule"; }
    std::string SingleLineTypes() const { return "SingleLine"; }
    std::tuple<std::string, std::string, std::string> CodeBlockTypes() const { return { "Start", "CodeBlok", "End" }; }

    bool IsVarBlockChecked() const { return m_isVarBlockChecked; }
    void SetVarBlockChecked() { m_isVarBlockChecked = true; }

    bool IsFirstProgName() const { return true; }

    std::set<std::string>& IdentTable() { return m_identTable; }

    const GeneratorDetails& Details() const { return m_details; }

private:
    std::set<std::string> m_identTable{};
    bool m_isVarBlockChecked = false;

    const GeneratorDetails m_details{ {
        .regPrefix = "",
        .numberType = "dw", .numberTypeExtended = "word",
        .argSize = 2,
        .numberStrType = "%hd"
        } };
};

enum class ItemType : uint32_t
{
	None = 0,
	Token = 1 << 0,
	Rule = 1 << 1,
	TokenAndRule = Token | Rule,
	Operand = 1 << 2,
	Operation = 1 << 3,
	EquationEnd = 1 << 4,
	LBracket = 1 << 5,
	RBracket = 1 << 6,

	Lsqr = 1 << 7, //LSquare 
	Rsqr = 1 << 8 // RSquare
};
DEFINE_ENUM_FLAG_OPERATORS(ItemType)

class Controller : public singleton<Controller>
{
public:
    static constexpr int NoPriority = std::numeric_limits<int>::min();

public:
    void init();

    template<typename T>
    void regItem(ItemType type = ItemType::TokenAndRule, int priority = NoPriority) const
    {
        auto item = std::make_shared<T>();
        regItem(item, item, type, priority);
    }

    void regUnchangedTextToken(std::shared_ptr<IToken> target, std::shared_ptr<IToken> lBorder, std::shared_ptr<IToken> rBorder) const;

    void regOperatorRule(const RuleMaker& rule, bool isNeedSemicolon = false);

    std::shared_ptr<IBackusRule> addRule(const std::string& name, const std::list<BackusRuleItem>& items) const;

    BackusRulePtr topRule();

    std::shared_ptr<Context> context() { return m_context; }

protected:
    Controller() { m_context = std::make_shared<Context>(); }

    void regItem(std::shared_ptr<IToken> token, std::shared_ptr<IBackusRule> rule, ItemType type, int priority) const;

    BackusRulePtr MakeTopRule(std::shared_ptr<Controller> controller) const;

private:
    BackusRulePtr m_topRule;
    std::set<std::string> m_operatorRuleNames;
    std::set<std::string> m_operatorRuleWithSemicolonNames;

    std::shared_ptr<Context> m_context;
};