#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"

class Or : public TokenBase<Or>, public BackusRuleBase<Or>, public GeneratorItemBase<Or>
{
    BASE_ITEM

public:
    Or() { setLexeme("or"); };
    virtual ~Or() = default;

    void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const final
    {
        RegPROC(details);
        out << "\tcall Or_\n";
    };

    static void RegPROC(GeneratorDetails& details)
    {
        if (!IsRegistered())
        {
            details.registerProc("Or_", PrintOr);
            SetRegistered();
        }
    }

private:
    static void PrintOr(std::ostream& out, const GeneratorDetails::GeneratorArgs& args)
    {
        out << ";===Procedure Or=============================================================================\n";
        out << "Or_ PROC\n";
        out << "\tpushf\n";
        out << "\tpop cx\n\n";
        out << "\tmov " << args.regPrefix << "ax, [esp + " << args.posArg0 << "]\n";
        out << "\tcmp " << args.regPrefix << "ax, 0\n";
        out << "\tjnz or_true\n";
        out << "\tjz or_t1\n";
        out << "or_t1:\n";
        out << "\tmov " << args.regPrefix << "ax, [esp + " << args.posArg1 << "]\n";
        out << "\tcmp " << args.regPrefix << "ax, 0\n";
        out << "\tjnz or_true\n";
        out << "or_false:\n";
        out << "\tmov " << args.regPrefix << "ax, 0\n";
        out << "\tjmp or_fin\n";
        out << "or_true:\n";
        out << "\tmov " << args.regPrefix << "ax, 1\n";
        out << "or_fin:\n";
        out << "\tpush cx\n";
        out << "\tpopf\n\n";
        GeneratorUtils::PrintResultToStack(out, args);
        out << "\tret\n";
        out << "Or_ ENDP\n";
        out << ";============================================================================================\n";
    }
};