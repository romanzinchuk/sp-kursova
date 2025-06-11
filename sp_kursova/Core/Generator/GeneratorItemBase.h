#pragma once
#include "stdafx.h"
#include "Core/Generator/GeneratorUtils.h"

template <class T>
class GeneratorItemBase : public IGeneratorItem
{
public:
    virtual ~GeneratorItemBase() = default;

    void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const override {};

protected:
    std::string customData_imp(const std::string& id) const { return m_customData[id]; }
    void setCustomData_imp(const std::string& data, const std::string& id) { m_customData[id] = data; }

    static bool IsRegistered() { return registered; }
    static void SetRegistered() { registered = true; }

    static bool registered;

private:
    mutable std::map<std::string, std::string> m_customData{ {"default",""} };
};

template<class T>
bool GeneratorItemBase<T>::registered = false;
