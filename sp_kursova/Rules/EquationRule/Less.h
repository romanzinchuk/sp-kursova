#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"

class Less : public TokenBase<Less>, public BackusRuleBase<Less>, public GeneratorItemBase<Less>
{
    BASE_ITEM

public:
    Less() { setLexeme("<="); };
    virtual ~Less() = default;

    void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const final
    {
        RegPROC(details);
        out << "\tcall Less_\n";
    };

    static void RegPROC(GeneratorDetails& details)
    {
        if (!IsRegistered())
        {
            details.registerProc("Less_", PrintLess);
            SetRegistered();
        }
    }

private:
    static void PrintLess(std::ostream& out, const GeneratorDetails::GeneratorArgs& args)
    {
        out << ";===Procedure Less===========================================================================\n";
        out << "Less_ PROC\n";
        out << "\tpushf\n";
        out << "\tpop cx\n\n";
        out << "\tmov " << args.regPrefix << "ax, [esp + " << args.posArg0 << "]\n";
        out << "\tcmp " << args.regPrefix << "ax, [esp + " << args.posArg1 << "]\n";
        out << "\tjg less_false\n";
        out << "\tmov " << args.regPrefix << "ax, 1\n";
        out << "\tjmp less_fin\n";
        out << "less_false:\n";
        out << "\tmov " << args.regPrefix << "ax, 0\n";
        out << "less_fin:\n";
        out << "\tpush cx\n";
        out << "\tpopf\n\n";
        GeneratorUtils::PrintResultToStack(out, args);
        out << "\tret\n";
        out << "Less_ ENDP\n";
        out << ";============================================================================================\n";

    }
};