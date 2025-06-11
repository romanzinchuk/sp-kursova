#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"

class Number : public TokenBase<Number>, public BackusRuleBase<Number>, public GeneratorItemBase<Number>
{
    BASE_ITEM

public:
    Number() { setLexeme(""); };
    virtual ~Number() = default;

    std::shared_ptr<IToken> tryCreateToken(std::string& lexeme) const override
    {
        for (char c : lexeme) {
            if (!isdigit(c)) {
                return nullptr;
            }
        }

        auto token = clone();
        token->setValue(lexeme);
        lexeme.clear();
        return token;
    }

    void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const final
    {
        out << "\tpush " << details.args().numberTypeExtended << " ptr " << value() << std::endl;
    };
};