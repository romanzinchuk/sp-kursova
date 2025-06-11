#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"

class EqualGreater : public TokenBase<EqualGreater>, public BackusRuleBase<EqualGreater>, public GeneratorItemBase<EqualGreater>
{
    BASE_ITEM

public:
    EqualGreater() { setLexeme("eg"); };
    virtual ~EqualGreater() = default;

    void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const final
    {
        RegPROC(details);
        out << "\tcall Equal_Greater_\n";
    };

    static void RegPROC(GeneratorDetails& details)
    {
        if (!IsRegistered())
        {
            details.registerProc("Equal_Greater_", PrintEqualGreater);
            SetRegistered();
        }
    }

private:
    static void PrintEqualGreater(std::ostream& out, const GeneratorDetails::GeneratorArgs& args)
    {
        out << ";===Procedure Equal Greater==========================================================================\n";
        out << "Equal_Greater_ PROC\n";
        out << "\tpushf\n";
        out << "\tpop cx\n\n";
        out << "\tmov " << args.regPrefix << "ax, [esp + " << args.posArg0 << "]\n";
        out << "\tcmp " << args.regPrefix << "ax, [esp + " << args.posArg1 << "]\n";
        out << "\tjl equal_greater_false\n";
        out << "\tmov " << args.regPrefix << "ax, 1\n";
        out << "\tjmp equal_greater_fin\n";
        out << "equal_greater_false:\n";
        out << "\tmov " << args.regPrefix << "ax, 0\n";
        out << "equal_greater_fin:\n";
        out << "\tpush cx\n";
        out << "\tpopf\n\n";
        GeneratorUtils::PrintResultToStack(out, args);
        out << "\tret\n";
        out << "Equal_Greater_ ENDP\n";
        out << ";============================================================================================\n";
    }
};