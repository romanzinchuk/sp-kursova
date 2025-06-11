#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"
#include "Rules/EquationRule/Greate.h"
#include "Rules/EquationRule/Not.h"
#include "Rules/EquationRule/Subtraction.h"

class DownTo : public TokenBase<DownTo>, public BackusRuleBase<DownTo>, public GeneratorItemBase<DownTo>
{
    BASE_ITEM

public:
    DownTo() { setLexeme("downto"); };
    virtual ~DownTo() = default;

    void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const final
    {
        Greate::RegPROC(details);
        Not::RegPROC(details);
        Subtraction::RegPROC(details);
        out << customData("startLabel") << ":" << std::endl;
        details.pushLoopLabel(customData("iterationLabel"), customData("endLabel"));

        it++;
        auto postForm = GeneratorUtils::Instance()->ConvertToPostfixForm(it, end);

        auto postIt = postForm.begin();
        auto postEnd = postForm.end();
        for (const auto& item : postForm)
            item->genCode(out, details, postIt, postEnd);

        out << "\tpush " << customData("ident") << std::endl;
        out << "\tcall Greate_" << std::endl;
        out << "\tcall Not_" << std::endl;
    };
};