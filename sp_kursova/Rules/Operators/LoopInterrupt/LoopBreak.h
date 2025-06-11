#pragma once
#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"

class LoopBreak : public TokenBase<LoopBreak>, public BackusRuleBase<LoopBreak>, public GeneratorItemBase<LoopBreak>
{
    BASE_ITEM

public:
    LoopBreak() { setLexeme("break"); };
    virtual ~LoopBreak() = default;

    void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const final
    {
        out << "\tjmp " << details.currentLoopLabel().second << std::endl;
    };
};