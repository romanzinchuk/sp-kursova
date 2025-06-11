#include "stdafx.h"
#include "StringRule.h"

#include "Rules/StringRule/String.h"

SimpleToken(Quotes, "\"");

BackusRulePtr MakeStringRule(std::shared_ptr<Controller> controller)
{
    using enum ItemType;

    controller->regUnchangedTextToken(std::make_shared<String>(), std::make_shared<Quotes>(), std::make_shared<Quotes>());
    controller->regItem<Quotes>(Rule);
    controller->regItem<String>(Rule);

    auto stringRule = controller->addRule("StringRule", {
        BackusRuleItem({  Quotes::Type()}, OnlyOne),
        BackusRuleItem({  String::Type()}, OnlyOne),
        BackusRuleItem({  Quotes::Type()}, OnlyOne)
        });

    stringRule->setPostHandler([](BackusRuleList::iterator&,
        BackusRuleList::iterator& it,
        BackusRuleList::iterator& end)
        {
            it = std::prev(it, 3);
            end = std::remove(it, end, *it);
            it++;
            end = std::remove(it, end, *it);
        });

    return stringRule;
}