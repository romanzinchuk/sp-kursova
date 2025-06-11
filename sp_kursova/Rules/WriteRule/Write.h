#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"
#include "Rules/StringRule/String.h"

class Write : public TokenBase<Write>, public BackusRuleBase<Write>, public GeneratorItemBase<Write>
{
    BASE_ITEM

public:
    Write() { setLexeme("put"); };
    virtual ~Write() = default;

    void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const final
    {
        if (auto string = std::dynamic_pointer_cast<String>(*std::next(it, 2)))
        {
            it = std::next(it, 2);
            string->genCode(out, details, it, end);
            it = std::next(it, 2);

            out << "\tinvoke WriteConsoleA, hConsoleOutput, ADDR " << string->stringName() << ", SIZEOF " << string->stringName() << " - 1, 0, 0\n";
        }
        else
        {
            RegPROC(details);

            auto postForm = GeneratorUtils::Instance()->ConvertToPostfixForm(it, end);

            auto postIt = postForm.begin();
            auto postEnd = postForm.end();
            while (postIt != postEnd) {
                auto item = *postIt;
                item->genCode(out, details, postIt, postEnd);
                postIt++;
            }

            out << "\tcall Output_\n";
        }
    };

    static void RegPROC(GeneratorDetails& details)
    {
        if (!IsRegistered())
        {
            details.registerRawData("OutMessage", "\tdb\t\"" + details.args().numberStrType + "\", 0");
            details.registerRawData("ResMessage", "\tdb\t20 dup (?)");
            details.registerProc("Output_", PrintOutput);
            SetRegistered();
        }
    }

private:
    static void PrintOutput(std::ostream& out, const GeneratorDetails::GeneratorArgs& args)
    {
        out << ";===Procedure Output=========================================================================\n";
        out << "Output_ PROC value: " << args.numberTypeExtended.c_str() << std::endl;
        out << "\tinvoke wsprintf, ADDR ResMessage, ADDR OutMessage, value\n";
        out << "\tinvoke WriteConsoleA, hConsoleOutput, ADDR ResMessage, eax, 0, 0\n";
        out << "invoke WriteConsoleA, hConsoleOutput, ADDR msg1310, SIZEOF msg1310 - 1, 0, 0\n";
        out << "\tret " << args.argSize << std::endl;
        out << "Output_ ENDP\n";
        out << ";============================================================================================\n";
    }
};