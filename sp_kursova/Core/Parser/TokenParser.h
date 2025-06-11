#pragma once
#include "stdafx.h"
#include "Utils/singleton.hpp"
#include "Core/Tokens/IToken.hpp"
#include "Utils/TablePrinter.h"

class TokenParser : public singleton<TokenParser>
{
public:
    static constexpr int NoPriority = std::numeric_limits<int>::min();

public:
    std::list<std::shared_ptr<IToken>> tokenize(std::istream& input);

    void regToken(std::shared_ptr<IToken> token, int priority = NoPriority);
    void regUnchangedTextToken(std::shared_ptr<IToken> target, std::shared_ptr<IToken> lBorder, std::shared_ptr<IToken> rBorder);

    template<class T>
    static void PrintTokens(std::ostream& out, const std::list<std::shared_ptr<T>>& tokens)
    {
        auto getNumCount = [](int k) { return std::to_string(k).size(); };

        size_t maxLemexeLen = 0;
        size_t maxTypeLen = 0;
        size_t maxValueLen = 0;

        for (auto token : tokens)
        {
            maxLemexeLen = std::max(maxLemexeLen, token->lexeme().size());
            maxTypeLen = std::max(maxTypeLen, token->type().size());
            maxValueLen = std::max(maxValueLen, token->value().size());
        }

        const std::string kHeaderColumn0 = "#";
        const std::string kHeaderColumn1 = "SYMBOL";
        const std::string kHeaderColumn2 = "TYPE";
        const std::string kHeaderColumn3 = "VALUE";
        const std::string kHeaderColumn4 = "LINE";

        size_t colPadding = 1;

        auto widthColumn0 = std::max(kHeaderColumn0.size(), getNumCount(tokens.size())) + 2 * colPadding;
        auto widthColumn1 = std::max(kHeaderColumn1.size(), maxLemexeLen) + 2 * colPadding;
        auto widthColumn2 = std::max(kHeaderColumn2.size(), maxTypeLen) + 2 * colPadding;
        auto widthColumn3 = std::max(kHeaderColumn3.size(), maxValueLen) + 2 * colPadding;
        auto widthColumn4 = std::max(kHeaderColumn4.size(), getNumCount(tokens.back()->line())) + 2 * colPadding;

        if ((kHeaderColumn0.size() % 2) != (widthColumn0 % 2)) widthColumn0++;
        if ((kHeaderColumn1.size() % 2) != (widthColumn1 % 2)) widthColumn1++;
        if ((kHeaderColumn2.size() % 2) != (widthColumn2 % 2)) widthColumn2++;
        if ((kHeaderColumn3.size() % 2) != (widthColumn3 % 2)) widthColumn3++;
        if ((kHeaderColumn4.size() % 2) != (widthColumn4 % 2)) widthColumn4++;

        size_t index = 1;
        auto getIndex = [&index](const std::shared_ptr<T>&) { return std::to_string(index++); };
        auto getLemexe = [](const std::shared_ptr<T>& token) { return token->lexeme(); };
        auto getType = [](const std::shared_ptr<T>& token) { return token->type(); };
        auto getValue = [](const std::shared_ptr<T>& token) { return token->value(); };
        auto getLine = [](const std::shared_ptr<T>& token) { return std::to_string(token->line()); };

        TablePrinter::PrintTable(out,
            { kHeaderColumn0, kHeaderColumn1, kHeaderColumn2, kHeaderColumn3, kHeaderColumn4 },
            { widthColumn0, widthColumn1, widthColumn2, widthColumn3, widthColumn4 },
            { TablePrinter::CENTRE, TablePrinter::RIGHT, TablePrinter::RIGHT , TablePrinter::RIGHT , TablePrinter::RIGHT },
            tokens,
            { getIndex, getLemexe, getType, getValue, getLine },
            colPadding);
    }

private:
    void throwIfTokenRegistered(std::shared_ptr<IToken> token);

    void recognizeToken(std::string& token, int curLine);
    bool isUnchangedTextTokenLast();

private:
    static bool IsNewLine(const char& ch);
    static bool IsTabulation(const char& ch);
    static bool IsAllowedSymbol(const char& ch);
    static bool IsAllowedSpecialSymbol(const char& ch);

private:
    struct PriorityCompare
    {
        bool operator()(const int& a, const int& b) const 
        {
            return a > b;
        }
    };

private:
    std::multimap<int, std::shared_ptr<IToken>, PriorityCompare> m_priorityTokens;
    std::map<std::string, std::tuple<std::shared_ptr<IToken>, std::shared_ptr<IToken>, std::shared_ptr<IToken>>> m_unchangedTextTokens;

    std::list<std::shared_ptr<IToken>> m_tokens;

    std::function<std::shared_ptr<IToken>(std::string)> m_getTokenByType = [this](const std::string& type) {
        auto start = m_priorityTokens.lower_bound(static_cast<int>(type.size()));
        auto mapItem = std::find_if(start, m_priorityTokens.end(), [&type](const auto& pair) { return pair.second->type() == type; });

        if (mapItem == m_priorityTokens.end())
            throw std::runtime_error("TokenParser::getTokenByType: Token with type " + type + " not found");

        return mapItem->second;
        };
};