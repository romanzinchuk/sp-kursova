#pragma once
#include "stdafx.h"
#include "Core/Backus/IBackusRule.h"

template <class T>
class BackusRuleBase : public IBackusRule
{
public:
    bool check(std::multimap<int, std::pair<std::string, std::vector<std::string>>>& errorsInfo,
        std::list<std::shared_ptr<IBackusRule>>::iterator& it,
        std::list<std::shared_ptr<IBackusRule>>::iterator& end) final
    {
        const auto xxx = type();
        const auto lin = (*it)->line();

        auto res = type() == (*it)->type();
        if (res)
            it++;
        return res;
    }

    void setPostHandler(const std::function<void(std::list<std::shared_ptr<IBackusRule>>::iterator& ruleBegin,
        std::list<std::shared_ptr<IBackusRule>>::iterator& it,
        std::list<std::shared_ptr<IBackusRule>>::iterator& end)>& handler) final { };
};