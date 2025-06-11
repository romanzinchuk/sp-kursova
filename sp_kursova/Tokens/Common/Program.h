#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"

class Program : public TokenBase<Program>, public BackusRuleBase<Program>, public GeneratorItemBase<Program>
{
    BASE_ITEM

public:
    Program() { setLexeme("program"); };
    virtual ~Program() = default;
};