#include "stdafx.h"
#include "Core/Parser/TokenParser.h"
#include "Utils/StringUtils.h"
#include "Tokens/Common/EndOfFile.h"

std::list<std::shared_ptr<IToken>> TokenParser::tokenize(std::istream& input)
{
    m_tokens.clear();

    int curLine = 1;
    std::string token;
    for (char ch; input.get(ch);)
    {
        if (!token.empty() && ((IsAllowedSymbol(token.front()) != IsAllowedSymbol(ch)) || IsTabulation(ch)))
            recognizeToken(token, curLine);

        if (IsNewLine(ch))
            ++curLine;

        if (isUnchangedTextTokenLast())
        {
            std::string unchangedTextTokenValue{ token };
            token.clear();
            int unchangedTextTokenLine{ curLine };

            const auto& [target, left, right] = m_unchangedTextTokens[m_tokens.back()->lexeme()];
            auto rBorderLex = right ? right->lexeme() : "\n";

            do
            {
                if (IsNewLine(ch))
                    ++curLine;

                unchangedTextTokenValue += ch;
            }
            while (!StringUtils::Compare(unchangedTextTokenValue, rBorderLex, StringUtils::EndWith) && input.get(ch));

            unchangedTextTokenValue = unchangedTextTokenValue.substr(0, unchangedTextTokenValue.size() - rBorderLex.size());
            m_tokens.push_back(target->tryCreateToken(unchangedTextTokenValue));
            m_tokens.back()->setLine(unchangedTextTokenLine);

            if (right)
            {
                m_tokens.push_back(right->tryCreateToken(rBorderLex));
                m_tokens.back()->setLine(curLine);
            }

            continue;
        }

        if (!IsTabulation(ch))
            token += ch;
    }

    if (!token.empty())
        recognizeToken(token, curLine);

    m_tokens.push_back(std::make_shared<EndOfFile>());
    return m_tokens;
}

void TokenParser::regToken(std::shared_ptr<IToken> token, int priority)
{
    throwIfTokenRegistered(token);

    if (priority == NoPriority)
        priority = static_cast<int>(token->lexeme().size());

    m_priorityTokens.insert(std::make_pair(priority, token));
}

void TokenParser::regUnchangedTextToken(std::shared_ptr<IToken> target, std::shared_ptr<IToken> lBorder, std::shared_ptr<IToken> rBorder)
{
    if(rBorder)
        throwIfTokenRegistered(rBorder);

    regToken(lBorder);
    throwIfTokenRegistered(target);

    m_unchangedTextTokens.try_emplace(lBorder->lexeme(), target, lBorder, rBorder);
}

void TokenParser::throwIfTokenRegistered(std::shared_ptr<IToken> token)
{
    auto start = m_priorityTokens.lower_bound(static_cast<int>(token->lexeme().size()));

    auto priorToken = std::find_if(start, m_priorityTokens.end(),
        [&token](const auto& pair) {
            return token->type() == pair.second->type();
        });

    auto unchTextToken = std::ranges::find_if(m_unchangedTextTokens,
        [&token](const auto& pair) {
            auto type = token->type();
            const auto& [main, left, right] = pair.second;
            return type == main->type() ||
                type == left->type() ||
                right && type == right->type();
        });

    if(priorToken != m_priorityTokens.end() || unchTextToken != m_unchangedTextTokens.end())
        throw std::runtime_error("TokenParser: Token with type " + token->type() + " already registered");
}

void TokenParser::recognizeToken(std::string& token, int curLine)
{
    if(m_priorityTokens.empty())
        throw std::runtime_error("TokenParser: No tokens registered");

    auto start = m_priorityTokens.lower_bound(static_cast<int>(token.size()));

    for (auto it = start; it != m_priorityTokens.end(); ++it)
    {
        auto curRegToken = it->second;
        if (auto newToken = curRegToken->tryCreateToken(token); newToken)
        {
            m_tokens.push_back(newToken);
            m_tokens.back()->setLine(curLine);
            break;
        }
    }

    if (!token.empty() && !isUnchangedTextTokenLast())
        recognizeToken(token, curLine);
}

bool TokenParser::isUnchangedTextTokenLast()
{
    if (!m_tokens.empty() && m_unchangedTextTokens.contains(m_tokens.back()->lexeme()))
    {
        auto const& [target, left, right] = m_unchangedTextTokens[m_tokens.back()->lexeme()];
        if (m_tokens.size() >= 2)
        {
            if (target->type() != (*(++m_tokens.rbegin()))->type())
                return true;
        }
        else
            return true;
    }

    return false;
}

bool TokenParser::IsNewLine(const char& ch)
{
    return ch == '\n';
}

bool TokenParser::IsTabulation(const char& ch)
{
    return ch == ' ' || ch == '\t' || IsNewLine(ch);
}

bool TokenParser::IsAllowedSymbol(const char& ch)
{
    return !!isalpha(ch) || !!isdigit(ch) || IsAllowedSpecialSymbol(ch);
}

bool TokenParser::IsAllowedSpecialSymbol(const char& ch)
{
    std::set<char> allowedSymblos{ '_' };
    return allowedSymblos.contains(ch);
}