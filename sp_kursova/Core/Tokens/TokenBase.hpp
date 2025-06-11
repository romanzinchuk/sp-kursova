#pragma once
#include "stdafx.h"
#include "Core/Tokens/IToken.hpp"

template <class T>
class TokenBase : public IToken
{
public:
    virtual ~TokenBase() = default;

    std::shared_ptr<IToken> tryCreateToken(std::string& lexem) const override
    {
        std::shared_ptr<IToken> token = nullptr;

        if (lexem.find(lexeme()) == 0)
        {
            lexem.erase(0, lexeme().size());
            token = clone();
            token->setValue(lexeme());
        }

        return token;
    };

    static std::string Type() { return GetTypeName<T>(); }

    void setLine(int line) final { m_line = line; };

    std::shared_ptr<IToken> clone() const final { return std::make_shared<T>(); };

protected:
    std::string lexeme_imp() const { return m_lexeme; };
    std::string type_imp() const { return Type(); }
    void setLexeme(const std::string& lexeme) { m_lexeme = lexeme; };
    std::string value_imp() const { return m_value; }
    int  line_imp() const { return m_line; };
    void setValue_imp(const std::string& value) { m_value = value; }

private:
    std::string m_lexeme;
    std::string m_value;
    int m_line = -1;
};
