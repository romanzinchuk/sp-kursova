#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"

class Subtraction : public TokenBase<Subtraction>, public BackusRuleBase<Subtraction>, public GeneratorItemBase<Subtraction>
{
    BASE_ITEM

public:
    Subtraction() { setLexeme("-"); };
    virtual ~Subtraction() = default;

    void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const final
    {
        RegPROC(details);
        out << "\tcall Sub_\n";
    };

    static void RegPROC(GeneratorDetails& details)
    {
        if (!IsRegistered())
        {
            details.registerProc("Sub_", PrintSub);
            SetRegistered();
        }
    }

private:
    static void PrintSub(std::ostream& out, const GeneratorDetails::GeneratorArgs& args)
    {
        out << ";===Procedure Sub============================================================================\n";
        out << "Sub_ PROC\n";
        out << "\tmov " << args.regPrefix << "ax, [esp + " << args.posArg0 << "]\n";
        out << "\tsub " << args.regPrefix << "ax, [esp + " << args.posArg1 << "]\n";
        GeneratorUtils::PrintResultToStack(out, args);
        out << "\tret\n";
        out << "Sub_ ENDP\n";
        out << ";============================================================================================\n";
    }
};