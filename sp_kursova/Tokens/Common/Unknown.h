#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"

namespace token
{
    class Unknown : public TokenBase<Unknown>, public BackusRuleBase<Unknown>, public GeneratorItemBase<Unknown>
    {
        BASE_ITEM

    public:
        Unknown() { setLexeme(""); };
        virtual ~Unknown() = default;

        std::shared_ptr<IToken> tryCreateToken(std::string& lexeme)  const override
        {
            auto token = clone();
            token->setValue(lexeme);
            lexeme.clear();
            return token;
        };
    };
}