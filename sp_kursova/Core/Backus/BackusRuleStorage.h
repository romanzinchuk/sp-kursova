#pragma once
#include "stdafx.h"
#include "Utils/singleton.hpp"
#include "Core/Backus/IBackusRule.h"

class BackusRuleStorage : public singleton<BackusRuleStorage>
{
public:
    void regRule(std::shared_ptr<IBackusRule> rule)
    {
        auto [it, inserted] = m_rules.try_emplace(rule->type(), rule);
        if (!inserted)
        {
            throw std::runtime_error("BackusRuleStorage::regRule: A rule with the type " + rule->type() + " already exists.");
        }
    }

    std::vector<std::shared_ptr<IBackusRule>> getRules(const std::vector<std::string>& ruleTypes) const
    {
        std::vector<std::shared_ptr<IBackusRule>> rules;

        for (const auto& ruleType : ruleTypes)
        {
            auto it = m_rules.find(ruleType);
            if (it == m_rules.end())
                throw std::runtime_error("BackusRuleStorage::regRule: A rule with the type " + ruleType + " not found.");

            rules.push_back(it->second);
        }

        return rules;
    };

private:
    std::map<std::string, std::shared_ptr<IBackusRule>> m_rules;
};