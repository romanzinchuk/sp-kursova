#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"

class Addition : public TokenBase<Addition>, public BackusRuleBase<Addition>, public GeneratorItemBase<Addition>
{
    BASE_ITEM

public:
    Addition() { setLexeme("+"); };
    virtual ~Addition() = default;

    void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const final
    {
        RegPROC(details);
        out << "\tcall Add_\n";
    };

    static void RegPROC(GeneratorDetails& details)
    {
        if (!IsRegistered())
        {
            details.registerProc("Add_", PrintAdd);
            SetRegistered();
        }
    }

private:
    static void PrintAdd(std::ostream& out, const GeneratorDetails::GeneratorArgs& args)
    {
        out << ";===Procedure Add============================================================================\n";
        out << "Add_ PROC\n";
        out << "\tmov " << args.regPrefix << "ax, [esp + " << args.posArg0 << "]\n";
        out << "\tadd " << args.regPrefix << "ax, [esp + " << args.posArg1 << "]\n";
        GeneratorUtils::PrintResultToStack(out, args);
        out << "\tret\n";
        out << "Add_ ENDP\n";
        out << ";============================================================================================\n";
    }
};