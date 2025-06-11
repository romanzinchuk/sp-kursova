#include "stdafx.h"
#include "Utils/TablePrinter.h"

void TablePrinter::PrintItem(std::ostream& out, const std::string& item, size_t width, Alignment alignment, size_t padding, bool lastItemInRow)
{
    out << "|";
    width -= 2 * padding;

    switch (alignment)
    {
    case RIGHT:
        out << std::string(padding, ' ') << std::setw(width) << std::right << item << std::string(padding, ' ');
        break;
    case LEFT:
        out << std::string(padding, ' ') << std::setw(width) << std::left << item << std::string(padding, ' ');
        break;
    case CENTRE:
    {
        if (auto paddingCenter = width - item.size() + 2 * padding)
        {
            int paddingRight = paddingCenter / 2;
            int paddingLeft = paddingCenter - paddingRight;
            out << std::setw(paddingLeft + item.size()) << item << std::setw(paddingRight) << " ";
        }
        else
        {
            out << std::string(padding, ' ') << item << std::string(padding, ' ');
        }
        break;
    }
    }

    if (lastItemInRow)
        out << "|";
}