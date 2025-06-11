#include "stdafx.h"
#include "EquationRule.h"

#include "Rules/IdentRule/Identifier.h"

#include "Rules/EquationRule/Number.h"

#include "Rules/EquationRule/Addition.h"
#include "Rules/EquationRule/Subtraction.h"

#include "Rules/EquationRule/Multiplication.h"
#include "Rules/EquationRule/Division.h"
#include "Rules/EquationRule/Mod.h"

#include "Rules/EquationRule/And.h"
#include "Rules/EquationRule/Or.h"

#include "Rules/EquationRule/Equal.h"
#include "Rules/EquationRule/Greate.h"
#include "Rules/EquationRule/Less.h"
#include "Rules/EquationRule/NotEqual.h"
#include "EqualGreater.h"

#include "Rules/EquationRule/Not.h"

BackusRulePtr MakeEquationRule(std::shared_ptr<Controller> controller)
{
    using enum ItemType;

    controller->regItem<Number>(TokenAndRule | Operand, 0);

    controller->regItem      <Addition>(TokenAndRule | Operation, 4);
    controller->regItem   <Subtraction>(TokenAndRule | Operation, 4);

    controller->regItem<Multiplication>(TokenAndRule | Operation, 5);
    controller->regItem      <Division>(TokenAndRule | Operation, 5);
    controller->regItem           <Mod>(TokenAndRule | Operation, 5);

    controller->regItem           <And>(TokenAndRule | Operation, 1);
    controller->regItem            <Or>(TokenAndRule | Operation, 1);

    controller->regItem         <Equal>(TokenAndRule | Operation, 2);
    controller->regItem         <EqualGreater>(TokenAndRule | Operation, 2);
    controller->regItem      <NotEqual>(TokenAndRule | Operation, 2);
    controller->regItem        <Greate>(TokenAndRule | Operation, 3);
    controller->regItem          <Less>(TokenAndRule | Operation, 3);

    controller->regItem           <Not>(TokenAndRule | Operation, 6);

    auto context = controller->context();
    auto equationRuleName = context->EquationRuleName();

    auto sign = controller->addRule("Sign", { BackusRuleItem({ Addition::Type(), Subtraction::Type() }, Optional) });
    auto signedNumber = controller->addRule("SignedNumber", {
        BackusRuleItem({  sign->type()}, Optional),
        BackusRuleItem({Number::Type()}, OnlyOne)
        });

    signedNumber->setPostHandler([](BackusRuleList::iterator&,
        BackusRuleList::iterator& it,
        BackusRuleList::iterator& end)
        {
            auto begRuleIt = std::prev(it, 2);
            auto optionalOperand = std::prev(it, 3);
            if ((*begRuleIt)->type() == Subtraction::Type()
                && !((*optionalOperand)->type() == Number::Type()
                || (*optionalOperand)->type() == Identifier::Type()))
            {
                it = begRuleIt;
                end = std::remove(it, end, *it);
                (*it)->setValue('-' + (*it)->value());
                it++;
            }
        });

    auto arithmetic = controller->addRule("Arithmetic", { BackusRuleItem({ Addition::Type(), Subtraction::Type() }, OnlyOne) });
    auto mult       = controller->addRule("Mult", { BackusRuleItem({ Multiplication::Type(), Division::Type(), Mod::Type() }, OnlyOne) });
    auto logic      = controller->addRule("Logic", { BackusRuleItem({ And::Type(), Or::Type() }, OnlyOne) });
    auto compare    = controller->addRule("Compare", { BackusRuleItem({ Equal::Type(),  Greate::Type(), Less::Type(), NotEqual::Type(), EqualGreater::Type()}, OnlyOne)});

    auto operationAndEquation = controller->addRule("OperationAndEquation", {
        BackusRuleItem({ mult->type(), arithmetic->type(), logic->type(), compare->type() }, OnlyOne),
        BackusRuleItem({ equationRuleName }, OnlyOne)
        });

    auto notRule = controller->addRule("NotRule", {
        BackusRuleItem({      Not::Type()}, OnlyOne),
        BackusRuleItem({ equationRuleName}, Optional | OneOrMore)
        });

    auto equationWithBrakets = controller->addRule("EquationWithBrakets", {
        BackusRuleItem({ Symbols::LBraket }, OnlyOne | PairStart),
        BackusRuleItem({ equationRuleName }, OnlyOne),
        BackusRuleItem({ Symbols::RBraket }, OnlyOne | PairEnd)
        });

    auto equation = controller->addRule(equationRuleName, {
        BackusRuleItem({signedNumber->type(), context->IdentRuleName(), notRule->type(), equationWithBrakets->type()},  OnlyOne),
        BackusRuleItem({operationAndEquation->type()}, Optional | OneOrMore)
        });

    return equation;
}