#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"
#include "Rules/EquationRule/Equal.h"
#include "Rules/EquationRule/Not.h"

class NotEqual : public TokenBase<NotEqual>, public BackusRuleBase<NotEqual>, public GeneratorItemBase<NotEqual>
{
    BASE_ITEM

public:
    NotEqual() { setLexeme("ne"); };
    virtual ~NotEqual() = default;

    void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const final
    {
        Equal::RegPROC(details);
        Not::RegPROC(details);
        out << "\tcall Equal_\n";
        out << "\tcall Not_\n";
    };
};