#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"
#include "Rules/EquationRule/Less.h"
#include "Rules/EquationRule/Not.h"
#include "Rules/EquationRule/Addition.h"

class To : public TokenBase<To>, public BackusRuleBase<To>, public GeneratorItemBase<To>
{
    BASE_ITEM

public:
    To() { setLexeme("to"); };
    virtual ~To() = default;

    void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const final
    {
        Less::RegPROC(details);
        Not::RegPROC(details);
        Addition::RegPROC(details);
        out << customData("startLabel") << ":" << std::endl;
        details.pushLoopLabel(customData("iterationLabel"), customData("endLabel"));

        it++;
        auto postForm = GeneratorUtils::Instance()->ConvertToPostfixForm(it, end);

        auto postIt = postForm.begin();
        auto postEnd = postForm.end();
        for (const auto& item : postForm)
            item->genCode(out, details, postIt, postEnd);

        out << "\tpush " << customData("ident") << std::endl;
        out << "\tcall Less_" << std::endl;
        out << "\tcall Not_" << std::endl;
    };
};