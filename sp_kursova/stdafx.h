#pragma once
#define NOMINMAX

#include <Windows.h>

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <variant>
#include <filesystem>
#include <memory>
#include <type_traits>
#include <limits>
#include <numeric>
#include <set>


#define BLOCK_COPY(classname)                            \
    private:                                             \
        classname(const classname&) = delete;            \
        classname& operator=(const classname&) = delete; \

#define BLOCK_MOVE(classname)                            \
    private:                                             \
        classname(const classname&&) = delete;           \
        classname& operator=(const classname&&) = delete

#define BLOCK_COPY_MOVE(classname) \
        BLOCK_COPY(classname)      \
        BLOCK_MOVE(classname)