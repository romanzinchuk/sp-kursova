#include "stdafx.h"
#include "ReadRule.h"

#include "Rules/ReadRule/Read.h"

BackusRulePtr MakeReadRule(std::shared_ptr<Controller> controller)
{
    controller->regItem<Read>();

    auto context = controller->context();

    auto read = controller->addRule("ReadRule", {
        BackusRuleItem({            Read::Type()}, OnlyOne),
        BackusRuleItem({        Symbols::LBraket}, OnlyOne),
        BackusRuleItem({ context->IdentRuleName()}, OnlyOne),
        BackusRuleItem({        Symbols::RBraket}, OnlyOne)
        });

    return read;
}