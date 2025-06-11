#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"

class Equal : public TokenBase<Equal>, public BackusRuleBase<Equal>, public GeneratorItemBase<Equal>
{
    BASE_ITEM

public:
    Equal() { setLexeme("=="); };
    virtual ~Equal() = default;

    void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const final
    {
        RegPROC(details);
        out << "\tcall Equal_\n";
    };

    static void RegPROC(GeneratorDetails& details)
    {
        if (!IsRegistered())
        {
            details.registerProc("Equal_", PrintEqual);
            SetRegistered();
        }
    }

private:
    static void PrintEqual(std::ostream& out, const GeneratorDetails::GeneratorArgs& args)
    {
        out << ";===Procedure Equal==========================================================================\n";
        out << "Equal_ PROC\n";
        out << "\tpushf\n";
        out << "\tpop cx\n\n";
        out << "\tmov " << args.regPrefix << "ax, [esp + " << args.posArg0 << "]\n";
        out << "\tcmp " << args.regPrefix << "ax, [esp + " << args.posArg1 << "]\n";
        out << "\tjne equal_false\n";
        out << "\tmov " << args.regPrefix << "ax, 1\n";
        out << "\tjmp equal_fin\n";
        out << "equal_false:\n";
        out << "\tmov " << args.regPrefix << "ax, 0\n";
        out << "equal_fin:\n";
        out << "\tpush cx\n";
        out << "\tpopf\n\n";
        GeneratorUtils::PrintResultToStack(out, args);
        out << "\tret\n";
        out << "Equal_ ENDP\n";
        out << ";============================================================================================\n";
    }
};