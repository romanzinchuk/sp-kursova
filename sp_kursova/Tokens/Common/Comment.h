#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Generator/GeneratorItemBase.h"

class Comment : public TokenBase<Comment>, public GeneratorItemBase<Comment>
{
    BASE_ITEM

public:
    Comment() { setLexeme(""); };
    virtual ~Comment() = default;

    std::shared_ptr<IToken> tryCreateToken(std::string& lexeme)  const override
    {
        auto token = clone();
        token->setValue(lexeme);
        lexeme.clear();
        return token;
    };
};