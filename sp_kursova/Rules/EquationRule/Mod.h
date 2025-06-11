#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"

class Mod : public TokenBase<Mod>, public BackusRuleBase<Mod>, public GeneratorItemBase<Mod>
{
    BASE_ITEM

public:
    Mod() { setLexeme("%"); };
    virtual ~Mod() = default;

    void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const final
    {
        RegPROC(details);
        out << "\tcall Mod_\n";
    };

    static void RegPROC(GeneratorDetails& details)
    {
        if (!IsRegistered())
        {
            details.registerStringData("ModErrMsg", "\\n" + Type() + ": Error: division by zero");
            details.registerProc("Mod_", PrintMod);
            SetRegistered();
        }
    }

private:
    static void PrintMod(std::ostream& out, const GeneratorDetails::GeneratorArgs& args)
    {
        out << ";===Procedure Mod============================================================================\n";
        out << "Mod_ PROC\n";
        out << "\tpushf\n";
        out << "\tpop cx\n\n";
        out << "\tmov " << args.regPrefix << "ax, [esp + " << args.posArg1 << "]\n";
        out << "\tcmp " << args.regPrefix << "ax, 0\n";
        out << "\tjne end_check\n";
        out << "\tinvoke WriteConsoleA, hConsoleOutput, ADDR ModErrMsg, SIZEOF ModErrMsg - 1, 0, 0\n";
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
        out << "\tmov " << args.regPrefix << "ax, " << args.regPrefix << "dx\n";
        out << "\tpush cx\n";
        out << "\tpopf\n\n";
        GeneratorUtils::PrintResultToStack(out, args);
        out << "\tret\n";
        out << "Mod_ ENDP\n";
        out << ";============================================================================================\n";
    }
};