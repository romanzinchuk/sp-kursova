#include "stdafx.h"
#include "Symbols.h"
#include "Utils/magic_enum.hpp"

bool operator==(const Symbols& lhs, const std::string& rhs) {
    return magic_enum::enum_name(lhs) == rhs;
}

bool operator==(const std::string& lhs, const Symbols& rhs) {
    return rhs == lhs;
}