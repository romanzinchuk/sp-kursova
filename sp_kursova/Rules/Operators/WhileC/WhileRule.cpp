#include "stdafx.h"
#include "WhileRule.h"

#include "Rules/Operators/WhileC/While.h"

BackusRulePtr MakeWhile(std::shared_ptr<Controller> controller)
{
    controller->regItem<While>();
    auto context = controller->context();
    static const auto [lStart, lCodeBlok, lEnd] = context->CodeBlockTypes();

    auto whileCStatement = controller->addRule("WhileStatement", {
       BackusRuleItem({              While::Type()}, OnlyOne),
       BackusRuleItem({           Symbols::LBraket}, OnlyOne),
       BackusRuleItem({ context->EquationRuleName()}, OnlyOne),
       BackusRuleItem({           Symbols::RBraket}, OnlyOne),
       BackusRuleItem({                  lCodeBlok}, OnlyOne)
        });

    whileCStatement->setPostHandler([](BackusRuleList::iterator& ruleBegin,
        BackusRuleList::iterator& it,
        BackusRuleList::iterator& end)
        {
            static size_t index = 0;
            index++;

            std::string startLabel = std::format("whileStart{}", index);
            std::string endLabel = std::format("whileEnd{}", index);

            (*ruleBegin)->setCustomData(startLabel, "startLabel");
            (*ruleBegin)->setCustomData(endLabel, "endLabel");

            size_t count = 0;
            for (auto itr = ruleBegin; itr != it; ++itr)
            {
                auto type = (*itr)->type();
                if (type == lStart)
                {
                    count++;
                }
                else if (type == lEnd && count == 1)
                {
                    (*itr)->setCustomData(std::format("\tjmp {}\n{}:", startLabel, endLabel));
                    (*itr)->setCustomData("pop","stack");
                    break;
                }
                else if (type == lEnd && count > 0)
                {
                    count--;
                }
            }
        });

    return whileCStatement;
}