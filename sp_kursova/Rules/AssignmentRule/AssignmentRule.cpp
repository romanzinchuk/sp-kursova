#include "stdafx.h"
#include "AssignmentRule.h"

#include "Rules/AssignmentRule/Assignment.h"

BackusRulePtr MakeAssignmentRule(std::shared_ptr<Controller> controller)
{
    controller->regItem<Assignment>();

    auto context = controller->context();

    auto assingmentRule = controller->addRule(context->AssignmentRuleName(), {
        BackusRuleItem({    context->IdentRuleName()}, OnlyOne),
        BackusRuleItem({         Assignment::Type()}, OnlyOne),
        BackusRuleItem({ context->EquationRuleName()}, OnlyOne)
        });

    return assingmentRule;
}
