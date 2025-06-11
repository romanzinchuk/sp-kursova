#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Generator/GeneratorItemBase.h"

class LComment : public TokenBase<LComment>, public GeneratorItemBase<LComment>
{
    BASE_ITEM

public:
    LComment() { setLexeme("/*"); };
    virtual ~LComment() = default;
};