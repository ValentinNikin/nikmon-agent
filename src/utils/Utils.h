#pragma once

#include <string>
#include <cstring>

namespace StringUtils {
    template<typename T, std::size_t N>
    inline bool startWith(const std::string& line, T (&prefix)[N]);
}

template<typename T, std::size_t N>
inline bool StringUtils::startWith(const std::string& line, T (&prefix)[N]) {
    return N == 1 ? false : !std::strncmp(line.c_str(), prefix, N - 1);
}