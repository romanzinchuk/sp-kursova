#include "stdafx.h"
#include "ForRule.h"

#include "Rules/Operators/For/For.h"
#include "Rules/Operators/For/To.h"
#include "Rules/Operators/For/DownTo.h"
#include "Rules/Operators/For/Do.h"


BackusRulePtr MakeFor(std::shared_ptr<Controller> controller)
{
    using enum ItemType;

    controller->regItem<For>();
    controller->regItem<To>(TokenAndRule | EquationEnd);
    controller->regItem<DownTo>(TokenAndRule | EquationEnd);
    controller->regItem<Do>(TokenAndRule | EquationEnd);

    auto context = controller->context();
    static const auto [lStart, lCodeBlok, lEnd] = context->CodeBlockTypes();
    static const auto codeLine = context->OperatorsRuleName();
    auto forToOrDownToDoRule = controller->addRule("ForToOrDownToDoRule", {
       BackusRuleItem({      For::Type()}, OnlyOne),
       BackusRuleItem({ "AssignmentRule"}, OnlyOne),
       BackusRuleItem({ To::Type(), DownTo::Type()}, OnlyOne),
       BackusRuleItem({ context->EquationRuleName()}, OnlyOne),
       BackusRuleItem({       Do::Type()}, OnlyOne),
       BackusRuleItem({      lCodeBlok}, OnlyOne)
        });

    forToOrDownToDoRule->setPostHandler([context](BackusRuleList::iterator& ruleBegin,
        BackusRuleList::iterator& it,
        BackusRuleList::iterator& end)
        {
            static size_t index = 0;
            index++;

            std::string startLabel = std::format("forPasStart{}", index);
            std::string endLabel = std::format("forPasEnd{}", index);
            std::string iterationLabel = std::format("forPasIteration{}", index);
            auto ident = *std::next(ruleBegin, 1);

            bool increment = false;
            for (auto itr = ruleBegin; itr != it; ++itr)
            {
                auto type = (*itr)->type();
                if ((type == To::Type() || type == DownTo::Type()))
                {
                    if (type == To::Type())
                        increment = true;

                    (*itr)->setCustomData(startLabel, "startLabel");
                    (*itr)->setCustomData(endLabel, "endLabel");
                    (*itr)->setCustomData(iterationLabel, "iterationLabel");
                    (*itr)->setCustomData(ident->customData(), "ident");
                    
                }
                else if (type == Do::Type())
                {
                    (*itr)->setCustomData(endLabel, "endLabel");
                    break;
                }
            }

            std::string code;

            code += std::format("\t{}:", iterationLabel);
            code += std::format("\tpush {}\n", ident->customData());
            code += std::format("\tpush {} ptr 1\n", context->Details().args().numberTypeExtended);
            code += std::format("\tcall {}\n", increment ? "Add_" : "Sub_");
            code += std::format("\tpop {}\n", ident->customData());
            code += std::format("\tjmp {}\n", startLabel);
            code += std::format("{}:", endLabel);

            auto ptr = (*std::prev(it, 1));
            ptr->setCustomData(code);
            ptr->setCustomData("pop", "stack");
        });

    return forToOrDownToDoRule;
}