#pragma once
#include "stdafx.h"

__interface IItem
{
    virtual std::string lexeme() const = 0;
    virtual std::string type() const = 0;
    virtual std::string value() const = 0;
    virtual void        setValue(const std::string & value) = 0;
    virtual int         line() const = 0;

    virtual std::string customData(const std::string& id = "default") const = 0;
    virtual void        setCustomData(const std::string& data, const std::string& id = "default") = 0;
};

#define BASE_ITEM public: \
    std::string lexeme() const final { return lexeme_imp(); } \
    std::string type()   const final { return type_imp(); } \
    void        setValue(const std::string& value) final { setValue_imp(value); } \
    std::string value()  const final { return value_imp(); } \
    int  line()          const final { return line_imp(); }; \
    std::string customData(const std::string& id = "default")   const final { return customData_imp(id); } \
    void        setCustomData(const std::string& data, const std::string& id = "default") final { setCustomData_imp(data, id); }