#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"

class Vars : public TokenBase<Vars>, public BackusRuleBase<Vars>, public GeneratorItemBase<Vars>
{
    BASE_ITEM

public:
    Vars() { setLexeme("var"); };
    virtual ~Vars() = default;
};