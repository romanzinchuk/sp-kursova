#pragma once
#include "stdafx.h"
#include "Core/IItem.h"

__interface IToken : public IItem
{
public:
    virtual std::shared_ptr<IToken> tryCreateToken(std::string& lexeme) const = 0;

    virtual void setLine(int line) = 0;

    virtual std::shared_ptr<IToken> clone() const = 0;
};

template <class T>
std::string GetTypeName()
{
    static std::string type;

    if (type.empty())
    {
        std::string name = typeid(T).name();
        type = { name.begin() + 6, name.end() };
    }

    return type;
}

