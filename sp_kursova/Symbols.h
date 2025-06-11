#pragma once
#include "stdafx.h"

enum class Symbols
{
    Underscore,

    Comma,
    Colon,
    Semicolon,

    LBraket,
    RBraket,

    Plus,
    Minus
};

bool operator==(const Symbols& lhs, const std::string& rhs);
bool operator==(const std::string& lhs, const Symbols& rhs);

#define SimpleToken(name, lexeme) \
class name : public TokenBase<name>, public BackusRuleBase<name>, public GeneratorItemBase<name> \
{ \
    BASE_ITEM \
\
public: \
    name() { setLexeme(lexeme); }; \
    ~name() final = default; \
};