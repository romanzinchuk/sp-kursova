#pragma once
#include "stdafx.h"
#include "Core/Backus/IBackusRule.h"
#include "BackusRuleStorage.h"
#include "Symbols.h"
#include "Utils/magic_enum.hpp"

class BackusRuleItem
{
public:
    BackusRuleItem(const std::vector<std::variant<std::string, Symbols>>& rules, RuleCountPolicy policy) : m_policy(policy)
    {
        for (auto rule : rules)
        {
            if (std::holds_alternative<std::string>(rule))
                m_ruleNames.push_back(std::get<std::string>(rule));
            else
                m_ruleNames.emplace_back(magic_enum::enum_name(std::get<Symbols>(rule)));
        }
    }

    std::vector<std::shared_ptr<IBackusRule>> rules() const
    {
        if (m_rules.empty())
            m_rules = BackusRuleStorage::Instance()->getRules(m_ruleNames);

        return m_rules;
    };

    RuleCountPolicy policy() const { return m_policy; };

private:
    std::vector<std::string> m_ruleNames;
    mutable std::vector<std::shared_ptr<IBackusRule>> m_rules;
    RuleCountPolicy m_policy = NoPolicy;
};