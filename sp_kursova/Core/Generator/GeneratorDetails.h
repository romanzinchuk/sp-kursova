#pragma once
#include "stdafx.h"

#include <stack>
#include <utility>


struct LoopLables {
    std::string first;
    std::string second;
};

std::stack<LoopLables>* getLoopStack();

class GeneratorDetails
{
    friend class Generator;
public:
    struct GeneratorArgs
    {
        std::string regPrefix;
        std::string numberType;
        std::string numberTypeExtended;
        size_t argSize;
        size_t posArg0;
        size_t posArg1;
        std::string numberStrType;
    };

public:
    explicit GeneratorDetails(const GeneratorArgs& args) : m_args(args)
    {
        m_args.posArg0 = m_kRetAddrSize + m_args.argSize;
        m_args.posArg1 = m_kRetAddrSize;
    }

    const GeneratorArgs& args() const { return m_args; }

    void registerNumberData(const std::string& name)
    {
        throwIfDataExists(name);
        m_userNumberData[name] = '\t' + name + '\t' + m_args.numberType + '\t' + "0";
    }

    void registerNumberArrayData(const std::string& name, const std::string& amount)
    {
        throwIfDataExists(name);
        m_userNumberData[name] = '\t' + name + '\t' + m_args.numberType + '\t' + amount + "\t" + "dup(0)";
    }

    void registerStringData(const std::string& name, const std::string& data)
    {
        throwIfDataExists(name);

        std::string item;
        size_t start = 0;
        size_t end;
        std::string delimiter = "\\n";

        m_userStringData[name] = '\t' + name + "\tdb\t";

        while ((end = data.find(delimiter, start)) != std::string::npos)
        {
            item = data.substr(start, end - start);
            if (!item.empty())
                m_userStringData[name] += "\"" + item + "\", ";
            m_userStringData[name] += "13, 10, ";
            start = end + delimiter.length();
        }

        item = data.substr(start);
        if (!item.empty())
            m_userStringData[name] += "\"" + item + "\", ";

        m_userStringData[name] += "0";
    }

    void registerRawData(const std::string& name, const std::string& rawData)
    {
        throwIfDataExists(name);
        m_userRawData[name] = '\t' + name + '\t' + rawData;
    }

    void registerProc(const std::string& type, const std::function<void(std::ostream& out, const GeneratorArgs&)>& generator)
    {
        if (!m_procGenerators.contains(type))
            m_procGenerators[type] = generator;
        else
            throw std::runtime_error("Proc for type " + type + " already exists");
    }

    LoopLables currentLoopLabel() {
        return getLoopStack()->top();
    }

    void pushLoopLabel(const std::string& startLabel, const std::string& endLabel) {
        getLoopStack()->emplace(LoopLables{ startLabel,endLabel});
    }

    void popLoopLabel() {
        getLoopStack()->pop();
    }

private:
    void throwIfDataExists(const std::string& name) const
    {
        if (m_userNumberData.contains(name) || m_userStringData.contains(name) || m_userRawData.contains(name))
            throw std::runtime_error("Data with name " + name + " already exists");
    }

private:
    GeneratorArgs m_args;

    std::map<std::string, std::string> m_userNumberData;
    std::map<std::string, std::string> m_userStringData;
    std::map<std::string, std::string> m_userRawData;
    std::map<std::string, std::function<void(std::ostream& out, const GeneratorArgs&)>> m_procGenerators;

    static constexpr size_t m_kRetAddrSize = 4;
};