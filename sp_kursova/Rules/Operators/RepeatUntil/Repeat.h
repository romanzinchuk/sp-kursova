#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"

class Repeat : public TokenBase<Repeat>, public BackusRuleBase<Repeat>, public GeneratorItemBase<Repeat>
{
    BASE_ITEM

public:
    Repeat() { setLexeme("repeat"); };
    virtual ~Repeat() = default;

    void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const final
    {
        out << customData("startLabel") << ":" << std::endl;
        details.pushLoopLabel(customData("startLabel"), customData("endLabel"));
    };
};