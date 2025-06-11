#pragma once
#include "stdafx.h"

class TablePrinter
{
public:
    enum Alignment
    {
        RIGHT,
        LEFT,
        CENTRE
    };

    template <template <typename, typename...> class ContainerType, typename T, typename... Args>
    static void PrintTable(std::ostream& out,
        const std::vector<std::string>& headers,
        const std::vector<size_t>& columnWidths,
        const std::vector<Alignment>& columnAlignments,
        const ContainerType<T, Args...>& items,
        const std::vector<std::function<std::string(const T&)>>& dataGetters,
        size_t padding = 0)
    {
        if(headers.size() != columnWidths.size() || headers.size() != columnAlignments.size() || headers.size() != dataGetters.size())
            throw std::invalid_argument("TablePrinter::PrintTable: Number of headers, column widths, column alignments and data getters must be the same");

        auto totalWidth = std::accumulate(columnWidths.begin(), columnWidths.end(), size_t(0)) + headers.size() * padding + 1;

        out << std::string(totalWidth, '=') << std::endl;
        for (size_t i = 0; i < headers.size(); i++)
        {
            PrintItem(out, headers[i], columnWidths[i], CENTRE, 0, i == headers.size() - 1);
        }
        out << std::endl;

        for (size_t i = 0; i < headers.size(); i++)
        {
            PrintItem(out, std::string(columnWidths[i], '='), columnWidths[i], CENTRE, 0, i == headers.size() - 1);
        }
        out << std::endl;

        for (const auto& item : items)
        {
            for (size_t i = 0; i < dataGetters.size(); i++)
            {
                PrintItem(out, dataGetters[i](item), columnWidths[i], columnAlignments[i], padding, i == headers.size() - 1);
            }
            out << std::endl;
        }
        out << std::string(totalWidth, '=') << std::endl;
    }

private:
    static void PrintItem(std::ostream& out, const std::string& item, size_t width, Alignment alignment, size_t padding = 0, bool lastItemInRow = false);
};

