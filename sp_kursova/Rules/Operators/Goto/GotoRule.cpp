#include "stdafx.h"
#include "GotoRule.h"

#include "Rules/Operators/Goto/Goto.h"
#include "Rules/Operators/Goto/Label.h"

#include "Rules/IdentRule/Identifier.h"
#include "Rules/IdentRule/Undefined.h"

static std::map<std::string, std::optional<BackusRuleList::iterator>> labelTable;

BackusRulePtr MakeLabel(std::shared_ptr<Controller> controller)
{
    using enum ItemType;

    controller->regItem<Label>(Rule);

    auto context = controller->context();
    static const auto [lStart, lCodeBlok, lEnd] = context->CodeBlockTypes();

    auto labelRule = controller->addRule("LabelRule", {
        BackusRuleItem({ context->SingleIdentRuleName()}, OnlyOne),
        BackusRuleItem({ Symbols::Colon}, OnlyOne)
        });

    
    labelRule->setPostHandler([context](BackusRuleList::iterator&,
        BackusRuleList::iterator& it,
        BackusRuleList::iterator& end)
        {
            it = std::prev(it, 2);
            auto identIt = it;
            auto identVal = (*identIt)->value();

            std::shared_ptr<IToken> label;
            if (context->IdentTable().contains((*identIt)->value()))
            {
                label = std::make_shared<Undefined>();
                label->setCustomData("Redefinition", "error");
            }
            else
                label = std::make_shared<Label>();

            label->setValue((*identIt)->value() + (*(++it))->value());
            end = std::remove(it, end, *it);
            label->setLine((*identIt)->line());
            label->setCustomData((*identIt)->customData());
            *identIt = std::dynamic_pointer_cast<IBackusRule>(label);

            if (!labelTable.contains(identVal))
            {
                labelTable.try_emplace(identVal, std::optional<BackusRuleList::iterator>());
            }
            else
            {
                if (auto optIt = labelTable[identVal]; optIt.has_value())
                {
                    auto gotoIdentIt = optIt.value();
                    if ((*gotoIdentIt)->type() == Undefined::Type())
                    {
                        auto labelName = std::make_shared<Identifier>();
                        labelName->setValue((*gotoIdentIt)->value());
                        labelName->setLine((*gotoIdentIt)->line());
                        labelName->setCustomData((*gotoIdentIt)->customData());
                        *gotoIdentIt = labelName;
                    }
                }
            }
        });

    return labelRule;
}

BackusRulePtr MakeGoto(std::shared_ptr<Controller> controller)
{
    controller->regItem<Goto>();

    auto context = controller->context();
    static const auto [lStart, lCodeBlok, lEnd] = context->CodeBlockTypes();

    auto gotoStatement = controller->addRule("GotoStatement", {
       BackusRuleItem({     Goto::Type()}, OnlyOne),
       BackusRuleItem({context->SingleIdentRuleName()}, OnlyOne)
        });

    gotoStatement->setPostHandler([](BackusRuleList::iterator&,
        BackusRuleList::iterator& it,
        BackusRuleList::iterator& end)
        {
            it = std::prev(it, 1);
            auto identIt = it;
            if (!labelTable.contains((*identIt)->value()))
            {
                if ((*identIt)->type() != Undefined::Type())
                {
                    auto undef = std::make_shared<Undefined>();
                    undef->setValue((*identIt)->value());
                    undef->setLine((*identIt)->line());
                    undef->setCustomData((*identIt)->customData());
                    *identIt = undef;
                }
                labelTable.try_emplace((*identIt)->value(), identIt);
            }
            else
            {
                auto ident = std::make_shared<Identifier>();
                ident->setValue((*identIt)->value());
                ident->setLine((*identIt)->line());
                ident->setCustomData((*identIt)->customData());
                *identIt = ident;
            }
            it = std::next(it);
        });

    return gotoStatement;
}