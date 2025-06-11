#include "stdafx.h"
#include "WriteRule.h"

// #include "Rules/StringRule/StringRule.h"

#include "Rules/WriteRule/Write.h"

BackusRulePtr MakeWriteRule(std::shared_ptr<Controller> controller)
{
    controller->regItem<Write>();

    auto context = controller->context();

    //auto stringRule = MakeStringRule(controller);

    auto write = controller->addRule("WriteRule", {
        BackusRuleItem({     Write::Type()}, OnlyOne),
        BackusRuleItem({  Symbols::LBraket}, OnlyOne | PairStart),
        BackusRuleItem({ /*stringRule->type(),*/ context->EquationRuleName()}, OnlyOne),
        BackusRuleItem({  Symbols::RBraket}, OnlyOne | PairEnd)
        });

    return write;
}