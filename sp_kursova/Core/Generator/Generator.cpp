#include "stdafx.h"
#include "Generator.h"
#include "GeneratorUtils.h"

static std::stack<LoopLables> loopStack;

std::stack<LoopLables>* getLoopStack() {
    return &loopStack;
}

void Generator::PrintBegin(std::ostream& out, GeneratorDetails& details)
{
    out << ".386\n";
    out << ".model flat, stdcall\n";
    out << "option casemap :none\n";
    out << std::endl;

    out << "include masm32\\include\\windows.inc\n";
    out << "include masm32\\include\\kernel32.inc\n";
    out << "include masm32\\include\\masm32.inc\n";
    out << "include masm32\\include\\user32.inc\n";
    out << "include masm32\\include\\msvcrt.inc\n";

    out << "includelib masm32\\lib\\kernel32.lib\n";
    out << "includelib masm32\\lib\\masm32.lib\n";
    out << "includelib masm32\\lib\\user32.lib\n";
    out << "includelib masm32\\lib\\msvcrt.lib\n";
}

void Generator::PrintData(std::ostream& out, GeneratorDetails& details)
{
    out << std::endl;
    out << ".DATA\n";
    out << ";===User Data================================================================================\n";

    for (const auto& [_, data] : details.m_userNumberData)
    {
        out << data << std::endl;
    }
    if (!details.m_userNumberData.empty())
        out << std::endl;

    for (const auto& [_, data] : details.m_userStringData)
    {
        out << data << std::endl;
    }
    if (!details.m_userStringData.empty())
        out << std::endl;

    out << ";===Addition Data============================================================================\n";
    out << "\t_gc\t\t\t\tdd\t?\n";
    out << "\thConsoleInput\tdd\t?\n";
    out << "\thConsoleOutput\tdd\t?\n";
    out << "\tendBuff\t\t\tdb\t5 dup (?)\n";
    out << "\tmsg1310\t\t\tdb\t13, 10, 0\n";

    if (!details.m_userRawData.empty())
        out << std::endl;

    for (const auto& [_, data] : details.m_userRawData)
    {
        out << data << std::endl;
    }
}

void Generator::PrintBeginCodeSegment(std::ostream& out, GeneratorDetails& details)
{
    out << std::endl;
    out << ".CODE\n";
    out << "start:\n";
    out << "invoke AllocConsole\n";
    out << "invoke GetStdHandle, STD_INPUT_HANDLE\n";
    out << "mov hConsoleInput, eax\n";
    out << "invoke GetStdHandle, STD_OUTPUT_HANDLE\n";
    out << "mov hConsoleOutput, eax\n";
    out << "xor ebx,ebx\n\n";
}

void Generator::PrintEnding(std::ostream& out, GeneratorDetails& details)
{
    out << "exit_label:\n";
    out << "invoke WriteConsoleA, hConsoleOutput, ADDR msg1310, SIZEOF msg1310 - 1, 0, 0\n";
    out << "invoke ReadConsoleA, hConsoleInput, ADDR endBuff, 5, 0, 0\n";
    out << "invoke ExitProcess, 0\n";

    for (const auto& [_, proc] : details.m_procGenerators)
    {
        out << std::endl << std::endl;
        proc(out, details.args());
    }

    out << "end start\n";
}

void Generator::genCode(std::ostream& out, GeneratorDetails& details,
    std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
    const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const
{
    for (; it != end; ++it)
    {
        (*it)->genCode(out, details, it, end);
    }
}