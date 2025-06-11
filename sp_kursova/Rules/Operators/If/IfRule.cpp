#include "stdafx.h"
#include "IfRule.h"

#include "Rules/Operators/If/If.h"
#include "Rules/Operators/If/Else.h"

BackusRulePtr MakeIf(std::shared_ptr<Controller> controller)
{
    controller->regItem<If>();
    controller->regItem<Else>();

    auto context = controller->context();
    static const auto [lStart, lCodeBlok, lEnd] = context->CodeBlockTypes();

    auto elseStatement = controller->addRule("ElseStatement", {
       BackusRuleItem({ Else::Type()}, OnlyOne),
       BackusRuleItem({    lCodeBlok}, OnlyOne),
        });

	auto ifStatement = controller->addRule("IfStatement", {
	   BackusRuleItem({       If::Type()}, OnlyOne),
	   BackusRuleItem({  context->EquationRuleName()}, OnlyOne),
	   BackusRuleItem({                   lCodeBlok}, OnlyOne),
	   BackusRuleItem({  elseStatement->type()}, Optional)
		});

        ifStatement->setPostHandler([](BackusRuleList::iterator& ruleBegin,
        BackusRuleList::iterator& it,
        BackusRuleList::iterator& end)
        {
            static size_t index = 0;
            index++;

            std::string elseLabel = std::format("elseLabel{}", index);
            std::string endLabel = std::format("endIf{}", index);

            bool hasElse = false;
            size_t count = 0;
            for (auto itr = ruleBegin; itr != it; ++itr)
            {
                auto type = (*itr)->type();
                if (type == lStart)
                {
                    count++;
                }
                else if (type == Else::Type() && count == 0)
                {
                    (*itr)->setCustomData(elseLabel, "elseLabel");
                    (*itr)->setCustomData(endLabel, "endLabel");
                    hasElse = true;
                }
                else if (type == lEnd && count == 1 && (*std::next(itr))->type() != Else::Type())
                {
                    (*itr)->setCustomData(endLabel + ':');
                    break;
                }
                else if (type == lEnd && count > 0)
                {
                    count--;
                }
            }

            (*ruleBegin)->setCustomData(hasElse ? elseLabel : endLabel, "label");
        });

   

        return ifStatement;
}