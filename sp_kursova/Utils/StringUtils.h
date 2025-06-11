#pragma once
#include "stdafx.h"

namespace StringUtils
{
    enum ComparisonMode : std::uint16_t
    {
        BeginsWith = 0,
        Contains,
        EndWith,
        Iequals
    };

    template <typename C, typename T>
    auto Compare(const std::basic_string<C>& str1, const T& str2, ComparisonMode mode, bool caseSensitive = true)
        -> typename std::enable_if_t<std::is_convertible_v<T, std::basic_string<C>>, bool>
    {
        std::basic_string<C> internalStr1{ str1 };
        std::basic_string<C> internalStr2{ str2 };

        if (!caseSensitive)
        {
            std::transform(internalStr1.begin(), internalStr1.end(), internalStr1.begin(), ::tolower);
            std::transform(internalStr2.begin(), internalStr2.end(), internalStr2.begin(), ::tolower);
        }

        switch (mode)
        {
        case ComparisonMode::BeginsWith:
            return internalStr1.find(internalStr2) == 0;
        case ComparisonMode::Contains:
            return internalStr1.find(internalStr2) != std::basic_string<C>::npos;
        case ComparisonMode::EndWith:
        {
            auto pos = internalStr1.rfind(internalStr2);
            return (pos == (internalStr1.length() - internalStr2.length())) && (pos != std::basic_string<C>::npos);
        }
        case ComparisonMode::Iequals:
            return internalStr1 == internalStr2;
        }

        return false;
    }
}