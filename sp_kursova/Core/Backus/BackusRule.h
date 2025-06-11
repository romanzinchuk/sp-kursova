#pragma once
#include "stdafx.h"
#include "Core/Backus/IBackusRule.h"
#include "BackusRuleItem.h"

class Controller;

class BackusRule : public IBackusRule
{
public:
    virtual ~BackusRule() = default;

    bool check(std::multimap<int, std::pair<std::string, std::vector<std::string>>>& errorsInfo,
        std::list<std::shared_ptr<IBackusRule>>::iterator& it,
        std::list<std::shared_ptr<IBackusRule>>::iterator& end) final;

    std::string type() const final { return m_name; };

    std::string lexeme() const final { return ""; };
    void        setValue(const std::string& value) final {};
    std::string value() const final { return ""; }
    int  line() const final { return -1; };
    std::string customData(const std::string& id) const final { return ""; }
    void        setCustomData(const std::string& data, const std::string& id) final {};

    void setPostHandler(const std::function<void(std::list<std::shared_ptr<IBackusRule>>::iterator& ruleBegin,
        std::list<std::shared_ptr<IBackusRule>>::iterator& it,
        std::list<std::shared_ptr<IBackusRule>>::iterator& end)>& handler) final
    {
        m_handler = handler;
    };

private:
    friend class Controller;

    static std::shared_ptr<IBackusRule> MakeRule(std::string name, std::list<BackusRuleItem> items);

    BackusRule(const std::string& name, const std::list<BackusRuleItem>& items) : m_name(name), m_backusItem(items) {}

    bool oneOrMoreCheck(std::multimap<int, std::pair<std::string, std::vector<std::string>>>& errorsInfo,
        std::list<std::shared_ptr<IBackusRule>>::iterator& it,
        std::list<std::shared_ptr<IBackusRule>>::iterator& end,
        const BackusRuleItem& item) const;

    bool checkItem(std::multimap<int, std::pair<std::string, std::vector<std::string>>>& errorsInfo,
        std::list<std::shared_ptr<IBackusRule>>::iterator& it,
        std::list<std::shared_ptr<IBackusRule>>::iterator& end,
        const BackusRuleItem& item) const;

    static bool HasFlag(RuleCountPolicy policy, RuleCountPolicy flag);

private:
    std::string m_name;
    std::list<BackusRuleItem> m_backusItem;
    std::function<void(std::list<std::shared_ptr<IBackusRule>>::iterator& ruleBegin,
        std::list<std::shared_ptr<IBackusRule>>::iterator& it,
        std::list<std::shared_ptr<IBackusRule>>::iterator& end)> m_handler;
};