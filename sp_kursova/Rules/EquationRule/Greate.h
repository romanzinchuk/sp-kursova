#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"

class Greate : public TokenBase<Greate>, public BackusRuleBase<Greate>, public GeneratorItemBase<Greate>
{
    BASE_ITEM

public:
    Greate() { setLexeme(">="); };
    virtual ~Greate() = default;

    void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const final
    {
        RegPROC(details);
        out << "\tcall Greate_\n";
    };

    static void RegPROC(GeneratorDetails& details)
    {
        if (!IsRegistered())
        {
            details.registerProc("Greate_", PrintGreate);
            SetRegistered();
        }
    }

private:
    static void PrintGreate(std::ostream& out, const GeneratorDetails::GeneratorArgs& args)
    {
        out << ";===Procedure Greate=========================================================================\n";
        out << "Greate_ PROC\n";
        out << "\tpushf\n";
        out << "\tpop cx\n\n";
        out << "\tmov " << args.regPrefix << "ax, [esp + " << args.posArg0 << "]\n";
        out << "\tcmp " << args.regPrefix << "ax, [esp + " << args.posArg1 << "]\n";
        out << "\tjl greate_false\n";
        out << "\tmov " << args.regPrefix << "ax, 1\n";
        out << "\tjmp greate_fin\n";
        out << "greate_false:\n";
        out << "\tmov " << args.regPrefix << "ax, 0\n";
        out << "greate_fin:\n";
        out << "\tpush cx\n";
        out << "\tpopf\n\n";
        GeneratorUtils::PrintResultToStack(out, args);
        out << "\tret\n";
        out << "Greate_ ENDP\n";
        out << ";============================================================================================\n";

    }
};