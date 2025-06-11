#pragma once
#include "stdafx.h"
#include "Utils/singleton.hpp"
#include "Core/Generator/GeneratorItemBase.h"

class Generator : public singleton<Generator>
{
public:
    template<class T>
    void generateCode(std::ostream& out, std::list<std::shared_ptr<T>>& items) const
    {
        if (!m_details)
            throw std::runtime_error("Generator details is not set");

        std::list<std::shared_ptr<IGeneratorItem>> generatorItems;
        for (auto item : items)
        {
            generatorItems.push_back(std::dynamic_pointer_cast<IGeneratorItem>(item));
        }
        auto it = generatorItems.begin();
        auto end = generatorItems.end();

        std::stringstream code;
        genCode(code, *m_details, it, end);

        PrintBegin(out, *m_details);
        PrintData(out, *m_details);
        PrintBeginCodeSegment(out, *m_details);
        out << code.str();
        PrintEnding(out, *m_details);
    }

    void setDetails(const GeneratorDetails& details) { m_details = std::make_shared<GeneratorDetails>(details); }

protected:
    Generator() = default;

private:
    void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const;

private:
    static void PrintBegin(std::ostream& out, GeneratorDetails& details);
    static void PrintData(std::ostream& out, GeneratorDetails& details);
    static void PrintBeginCodeSegment(std::ostream& out, GeneratorDetails& details);
    static void PrintEnding(std::ostream& out, GeneratorDetails& details);

private:
    std::shared_ptr<GeneratorDetails> m_details;
};