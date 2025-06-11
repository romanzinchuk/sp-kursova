#pragma once
#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"

class LoopContinue : public TokenBase<LoopContinue>, public BackusRuleBase<LoopContinue>, public GeneratorItemBase<LoopContinue>
{
    BASE_ITEM

public:
    LoopContinue() { setLexeme("continue"); };
    virtual ~LoopContinue() = default;

    void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const final
    {
        const auto x = details.currentLoopLabel().second;
        out << "\tjmp " << details.currentLoopLabel().first << std::endl;
    };
};