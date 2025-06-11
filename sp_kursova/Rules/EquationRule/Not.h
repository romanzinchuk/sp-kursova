#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"

class Not : public TokenBase<Not>, public BackusRuleBase<Not>, public GeneratorItemBase<Not>
{
    BASE_ITEM

public:
    Not() { setLexeme("!"); };
    virtual ~Not() = default;

    void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const final
    {
        RegPROC(details);
        out << "\tcall Not_\n";
    };

    static void RegPROC(GeneratorDetails& details)
    {
        if (!IsRegistered())
        {
            details.registerProc("Not_", PrintNot);
            SetRegistered();
        }
    }

private:
    static void PrintNot(std::ostream& out, const GeneratorDetails::GeneratorArgs& args)
    {
        out << ";===Procedure Not============================================================================\n";
        out << "Not_ PROC\n";
        out << "\tpushf\n";
        out << "\tpop cx\n\n";
        out << "\tmov " << args.regPrefix << "ax, [esp + " << args.posArg1 << "]\n";
        out << "\tcmp " << args.regPrefix << "ax, 0\n";
        out << "\tjnz not_false\n";
        out << "not_t1:\n";
        out << "\tmov " << args.regPrefix << "ax, 1\n";
        out << "\tjmp not_fin\n";
        out << "not_false:\n";
        out << "\tmov " << args.regPrefix << "ax, 0\n";
        out << "not_fin:\n";
        out << "\tpush cx\n";
        out << "\tpopf\n\n";
        out << "\tmov [esp + " << args.posArg1 << "], " << args.regPrefix << "ax\n";
        out << "\tret\n";
        out << "Not_ ENDP\n";
        out << ";============================================================================================\n";
    }
};