#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"

class Undefined : public TokenBase<Undefined>, public BackusRuleBase<Undefined>, public GeneratorItemBase<Undefined>
{
    BASE_ITEM

public:
    Undefined() { setLexeme(""); };
    virtual ~Undefined() = default;

    std::shared_ptr<IToken> tryCreateToken(std::string& lexeme)  const override
    { 
        auto token = clone();
        token->setValue(lexeme);
        lexeme.clear();
        return token;
    };
};