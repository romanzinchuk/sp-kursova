#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"

class Division : public TokenBase<Division>, public BackusRuleBase<Division>, public GeneratorItemBase<Division>
{
    BASE_ITEM

public:
    Division() { setLexeme("/"); };
    virtual ~Division() = default;

    void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const final
    {
        RegPROC(details);
        out << "\tcall Div_\n";
    };

    static void RegPROC(GeneratorDetails& details)
    {
        if (!IsRegistered())
        {
            details.registerStringData("DivErrMsg", "\\n" + Type() + ": Error: division by zero");
            details.registerProc("Div_", PrintDiv);
            SetRegistered();
        }
    }

private:
    static void PrintDiv(std::ostream& out, const GeneratorDetails::GeneratorArgs& args)
    {
        out << ";===Procedure Div============================================================================\n";
        out << "Div_ PROC\n";
        out << "\tpushf\n";
        out << "\tpop cx\n\n";
        out << "\tmov " << args.regPrefix << "ax, [esp + " << args.posArg1 << "]\n";
        out << "\tcmp " << args.regPrefix << "ax, 0\n";
        out << "\tjne end_check\n";
        out << "\tinvoke WriteConsoleA, hConsoleOutput, ADDR DivErrMsg, SIZEOF DivErrMsg - 1, 0, 0\n";
        out << "\tjmp exit_label\n";
        out << "end_check:\n";
        out << "\tmov " << args.regPrefix << "ax, [esp + " << args.posArg0 << "]\n";
        out << "\tcmp " << args.regPrefix << "ax, 0\n";
        out << "\tjge gr\n";
        out << "lo:\n";
        out << "\tmov " << args.regPrefix << "dx, -1\n";
        out << "\tjmp less_fin\n";
        out << "gr:\n";
        out << "\tmov " << args.regPrefix << "dx, 0\n";
        out << "less_fin:\n";
        out << "\tmov " << args.regPrefix << "ax, [esp + " << args.posArg0 << "]\n";
        out << "\tidiv " << args.numberTypeExtended << " ptr [esp + " << args.posArg1 << "]\n";
        out << "\tpush cx\n";
        out << "\tpopf\n\n";
        GeneratorUtils::PrintResultToStack(out, args);
        out << "\tret\n";
        out << "Div_ ENDP\n";
        out << ";============================================================================================\n";
    }
};