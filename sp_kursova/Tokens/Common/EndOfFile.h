#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"

class EndOfFile : public TokenBase<EndOfFile>, public BackusRuleBase<EndOfFile>, public GeneratorItemBase<EndOfFile>
{
    BASE_ITEM

public:
    EndOfFile() { setLexeme(""); };
    virtual ~EndOfFile() = default;
};