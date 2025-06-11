#pragma once
#include "stdafx.h"
#include "BackusRule.h"

std::shared_ptr<IBackusRule> BackusRule::MakeRule(std::string name, std::list<BackusRuleItem> items)
{
    struct EnableMakeShared : public BackusRule { EnableMakeShared(const std::string& name, const std::list<BackusRuleItem>& items) : BackusRule(name, items) {} };

    return std::make_shared<EnableMakeShared>(name, items);
}

bool BackusRule::check(std::multimap<int, std::pair<std::string, std::vector<std::string>>>& errorsInfo,
    std::list<std::shared_ptr<IBackusRule>>::iterator& it,
    std::list<std::shared_ptr<IBackusRule>>::iterator& end)
{
    bool res = true;
    bool pairItem = false;
    auto ruleBegin = it;
    auto this_line = it._Ptr->_Myval->line();
    for (auto item = m_backusItem.begin(); item != m_backusItem.end(); ++item)
    {
        if (it == end || !pairItem && HasFlag(item->policy(), RuleCountPolicy::PairEnd))
        {
            if (!HasFlag(item->policy(), RuleCountPolicy::Optional) || item != m_backusItem.end())
            {
                std::vector<std::string> types;

                for (const auto& rule : item->rules())
                    types.push_back(rule->type());

                errorsInfo.emplace((*it)->line(), std::make_pair((*it)->value(), types));
                res = false;
            }
            break;
        }

        if (pairItem && HasFlag(item->policy(), RuleCountPolicy::PairEnd) || !HasFlag(item->policy(), RuleCountPolicy::PairEnd))
        {
            bool resItem = true;
            auto startIt = it;
            if (HasFlag(item->policy(), RuleCountPolicy::Several))
                resItem = oneOrMoreCheck(errorsInfo, it, end, *item);
            else
                resItem = checkItem(errorsInfo, it, end, *item);

            if (!resItem  && (!HasFlag(item->policy(), RuleCountPolicy::Optional) || startIt != it))
            {
                res &= resItem;
                break;
            }

            if (resItem && HasFlag(item->policy(), RuleCountPolicy::PairStart))
            {
                pairItem = true;
            }

            if (resItem && pairItem && HasFlag(item->policy(), RuleCountPolicy::PairEnd))
            {
                pairItem = false;
            }
        }
    }

    if (res && m_handler)
        m_handler(ruleBegin, it, end);

    return res;
}

bool BackusRule::oneOrMoreCheck(std::multimap<int, std::pair<std::string, std::vector<std::string>>>& errorsInfo,
    std::list<std::shared_ptr<IBackusRule>>::iterator& it,
    std::list<std::shared_ptr<IBackusRule>>::iterator& end,
    const BackusRuleItem& item) const
{
    bool res = true;
    bool resItem = true;
    while (resItem && it != end && HasFlag(item.policy(), RuleCountPolicy::Several))
    {
        auto startIt = it;
        res &= resItem;
        resItem = checkItem(errorsInfo, it, end, item);

        if (!resItem && startIt != it)
            res = false;
    }

    return res;
}

bool BackusRule::checkItem(std::multimap<int, std::pair<std::string, std::vector<std::string>>>& errorsInfo,
    std::list<std::shared_ptr<IBackusRule>>::iterator& it,
    std::list<std::shared_ptr<IBackusRule>>::iterator& end,
    const BackusRuleItem& item) const
{
    bool res = false;
    std::vector<std::string> types;

    auto startIt = it;
    auto maxIt = it;
    if (it != end)
    {
        std::multimap<int, std::pair<std::string, std::vector<std::string>>> errors;
        for (auto rule : item.rules())
        {
            types.push_back(rule->type());
           
            if (!res && startIt == it)
            {
                res = rule->check(errors, it, end);
            }
            
            if (res)
            {
                break;
            }
            else if (!res && startIt != it)
            {
                if(std::distance(maxIt, end) > std::distance(it, end))
                    maxIt = it;

                it = startIt;
                errorsInfo.insert(errors.begin(), errors.end());
            }
        }
    }

    if (std::distance(maxIt, end) < std::distance(it, end))
        it = maxIt;

    if (!res)
        errorsInfo.emplace((*startIt)->line(), std::make_pair((*it)->value(), types));
    else
        errorsInfo.clear();

    return res;
}

bool BackusRule::HasFlag(RuleCountPolicy policy, RuleCountPolicy flag)
{
    return (policy & flag) == flag;
}