#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"

class Read :public TokenBase<Read>, public BackusRuleBase<Read>, public GeneratorItemBase<Read>
{
    BASE_ITEM

public:
    Read() { setLexeme("get"); };
    virtual ~Read() = default;

    void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const final
    {
        RegPROC(details);

        it = std::next(it, 2);

        out << "\tcall Input_\n";
        out << "\tmov " << (*it)->customData() << ", " << details.args().regPrefix << "ax\n";

        it = std::next(it, 2);
    };

    static void RegPROC(GeneratorDetails& details)
    {
        if (!IsRegistered())
        {
            details.registerRawData("InputBuf", "\tdb\t15 dup (?)");
            details.registerRawData("CharsReadNum", "dd\t?");
            details.registerProc("Input_", PrintInput);
            SetRegistered();
        }
    }

private:
    static void PrintInput(std::ostream& out, const GeneratorDetails::GeneratorArgs& args)
    {
        out << ";===Procedure Input==========================================================================\n";
        out << "Input_ PROC\n";
        out << "\tinvoke ReadConsoleA, hConsoleInput, ADDR InputBuf, 13, ADDR CharsReadNum, 0\n";
        out << "\tinvoke crt_atoi, ADDR InputBuf\n";
        out << "\tret\n";
        out << "Input_ ENDP\n";
        out << ";============================================================================================\n";
    }
};