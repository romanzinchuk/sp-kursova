#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"

class Multiplication : public TokenBase<Multiplication>, public BackusRuleBase<Multiplication>, public GeneratorItemBase<Multiplication>
{
    BASE_ITEM

public:
    Multiplication() { setLexeme("*"); };
    virtual ~Multiplication() = default;

    void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const final
    {
        RegPROC(details);
        out << "\tcall Mul_\n";
    };

    static void RegPROC(GeneratorDetails& details)
    {
        if (!IsRegistered())
        {
            details.registerProc("Mul_", PrintMul);
            SetRegistered();
        }
    }

private:
    static void PrintMul(std::ostream& out, const GeneratorDetails::GeneratorArgs& args)
    {
        out << ";===Procedure Mul============================================================================\n";
        out << "Mul_ PROC\n";
        out << "\tmov " << args.regPrefix << "ax, [esp + " << args.posArg0 << "]\n";
        out << "\timul " << args.numberTypeExtended << " ptr [esp + " << args.posArg1 << "]\n";
        GeneratorUtils::PrintResultToStack(out, args);
        out << "\tret\n";
        out << "Mul_ ENDP\n";
        out << ";============================================================================================\n";
    }
};