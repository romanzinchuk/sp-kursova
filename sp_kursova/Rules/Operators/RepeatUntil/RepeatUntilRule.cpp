#include "stdafx.h"
#include "RepeatUntilRule.h"

#include "Rules/Operators/RepeatUntil/Repeat.h"
#include "Rules/Operators/RepeatUntil/Until.h"

BackusRulePtr MakeRepeatUntil(std::shared_ptr<Controller> controller)
{
    controller->regItem<Repeat>();
    controller->regItem<Until>();

    auto context = controller->context();
    static const auto [lStart, lCodeBlok, lEnd] = context->CodeBlockTypes();
    auto operatorsRuleName = context->OperatorsRuleName();

    auto repeatUntilRule = controller->addRule("RepeatUntilRule", {
       BackusRuleItem({   Repeat::Type()}, OnlyOne),
       BackusRuleItem({operatorsRuleName}, OnlyOne),
       BackusRuleItem({    Until::Type()}, OnlyOne),
       BackusRuleItem({ Symbols::LBraket}, OnlyOne),
       BackusRuleItem({ context->EquationRuleName()}, OnlyOne),
       BackusRuleItem({ Symbols::RBraket}, OnlyOne)
        });

    repeatUntilRule->setPostHandler([](BackusRuleList::iterator& ruleBegin,
        BackusRuleList::iterator& it,
        BackusRuleList::iterator& end)
        {
            static size_t index = 0;
            index++;

            std::string startLabel = std::format("repeatStart{}", index);
            std::string endLabel = std::format("repeatEnd{}", index);

            (*ruleBegin)->setCustomData(startLabel, "startLabel");

            size_t count = 0;
            for (auto itr = ruleBegin; itr != it; ++itr)
            {
                auto type = (*itr)->type();
                if (type == Repeat::Type())
                {
                    (*itr)->setCustomData(startLabel, "startLabel");
                    (*itr)->setCustomData(endLabel, "endLabel");
                    count++;
                }
                else if (type == Until::Type() && count == 1)
                {
                    count--;
                    (*itr)->setCustomData(startLabel, "startLabel");
                    (*itr)->setCustomData(endLabel, "endLabel");
                    break;
                }
                else if (type == Until::Type() && count > 0)
                {
                    count--;
                }
            }
        });

    return repeatUntilRule;
}