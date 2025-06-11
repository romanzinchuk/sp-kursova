#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"

class String : public TokenBase<String>, public BackusRuleBase<String>, public GeneratorItemBase<String>
{
    BASE_ITEM

public:
    String() { setLexeme(""); };
    virtual ~String() = default;

    std::string stringName() const { return m_stringName; };

    std::shared_ptr<IToken> tryCreateToken(std::string& lexeme)  const override
    {
        auto token = clone();
        token->setValue(lexeme);
        lexeme.clear();
        return token;
    };

    void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const final
    {
        m_stringName = std::format("String_{}", index++);
        details.registerStringData(m_stringName, value());
    };

private:
    mutable std::string m_stringName;
    static size_t index;
};