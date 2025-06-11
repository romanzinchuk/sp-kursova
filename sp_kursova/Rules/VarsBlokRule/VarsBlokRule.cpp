#include "stdafx.h"
#include "VarsBlokRule.h"

#include "Rules/VarsBlokRule/VarType.h"

BackusRulePtr MakeVarsBlokRule(std::shared_ptr<Controller> controller)
{
    controller->regItem<VarType>();

    auto context = controller->context();

    auto commaAndIdentifier = controller->addRule("CommaAndIdentifier", {
        BackusRuleItem({          Symbols::Comma}, OnlyOne),
        BackusRuleItem({ context->IdentRuleName()}, OnlyOne)
        });

    auto varsBlok = controller->addRule("VarsBlok", {
        BackusRuleItem({           VarType::Type()}, OnlyOne),
        BackusRuleItem({  context->IdentRuleName()}, OnlyOne),
        BackusRuleItem({commaAndIdentifier->type()}, Optional | OneOrMore),
        BackusRuleItem({        Symbols::Semicolon}, OnlyOne)
        });

    varsBlok->setPostHandler([context](BackusRuleList::iterator&, BackusRuleList::iterator&, BackusRuleList::iterator&)
        {
            auto isVarBlockChecked = context->IsVarBlockChecked();

            context->SetVarBlockChecked();
        });

    return varsBlok;
}