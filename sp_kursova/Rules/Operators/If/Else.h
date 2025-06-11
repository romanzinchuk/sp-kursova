#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"

class Else : public TokenBase<Else>, public BackusRuleBase<Else>, public GeneratorItemBase<Else>
{
    BASE_ITEM

public:
    Else() { setLexeme("else"); };
    virtual ~Else() = default;

    void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const final
    {
        out << "\tjmp " << customData("endLabel") << std::endl;
        out << customData("elseLabel") << ":\n";
    };
};