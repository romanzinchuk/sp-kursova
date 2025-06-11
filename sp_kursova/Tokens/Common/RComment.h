#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Generator/GeneratorItemBase.h"

class RComment : public TokenBase<RComment>, public GeneratorItemBase<RComment>
{
    BASE_ITEM

public:
    RComment() { setLexeme("*/"); };
    virtual ~RComment() = default;
};