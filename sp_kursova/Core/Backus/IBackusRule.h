#pragma once
#include "stdafx.h"
#include "Core/IItem.h"

enum RuleCountPolicy : std::uint16_t
{
    NoPolicy = 0,
    Optional = 1 << 0,
    OnlyOne = 1 << 1,
    Several = 1 << 2,
    OneOrMore = OnlyOne | Several,
    PairStart = 1 << 3,
    PairEnd = 1 << 4,
};

DEFINE_ENUM_FLAG_OPERATORS(RuleCountPolicy)


__interface IBackusRule : public IItem
{
    virtual bool check(std::multimap<int, std::pair<std::string, std::vector<std::string>>>& errorsInfo,
        std::list<std::shared_ptr<IBackusRule>>::iterator& it,
        std::list<std::shared_ptr<IBackusRule>>::iterator& end) = 0;

    virtual void setPostHandler(const std::function<void(std::list<std::shared_ptr<IBackusRule>>::iterator& ruleBegin,
        std::list<std::shared_ptr<IBackusRule>>::iterator& it,
        std::list<std::shared_ptr<IBackusRule>>::iterator& end)>& handler) = 0;
};