#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"

class For : public TokenBase<For>, public BackusRuleBase<For>, public GeneratorItemBase<For>
{
    BASE_ITEM

public:
    For() { setLexeme("for"); };
    virtual ~For() = default;
};