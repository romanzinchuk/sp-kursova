#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"

class And : public TokenBase<And>, public BackusRuleBase<And>, public GeneratorItemBase<And>
{
    BASE_ITEM

public:
    And() { setLexeme("and"); };
    virtual ~And() = default;

    void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const final
    {
        RegPROC(details);
        out << "\tcall And_\n";
    };

    static void RegPROC(GeneratorDetails& details)
    {
        if (!IsRegistered())
        {
            details.registerProc("And_", PrintAnd);
            SetRegistered();
        }
    }

private:
    static void PrintAnd(std::ostream& out, const GeneratorDetails::GeneratorArgs& args)
    {
        out << ";===Procedure And============================================================================\n";
        out << "And_ PROC\n";
        out << "\tpushf\n";
        out << "\tpop cx\n\n";
        out << "\tmov " << args.regPrefix << "ax, [esp + " << args.posArg0 << "]\n";
        out << "\tcmp " << args.regPrefix << "ax, 0\n";
        out << "\tjnz and_t1\n";
        out << "\tjz and_false\n";
        out << "and_t1:\n";
        out << "\tmov " << args.regPrefix << "ax, [esp + " << args.posArg1 << "]\n";
        out << "\tcmp " << args.regPrefix << "ax, 0\n";
        out << "\tjnz and_true\n";
        out << "and_false:\n";
        out << "\tmov " << args.regPrefix << "ax, 0\n";
        out << "\tjmp and_fin\n";
        out << "and_true:\n";
        out << "\tmov " << args.regPrefix << "ax, 1\n";
        out << "and_fin:\n";
        out << "\tpush cx\n";
        out << "\tpopf\n\n";
        GeneratorUtils::PrintResultToStack(out, args);
        out << "\tret\n";
        out << "And_ ENDP\n";
        out << ";============================================================================================\n";
    }
};