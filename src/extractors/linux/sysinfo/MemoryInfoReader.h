#pragma once

#include "gtest/gtest.h"

#include <string>

struct MemoryInfo {
    int64_t totalSize {0};
    int64_t available {0};
    int64_t free {0};

    int64_t swapTotalSize {0};
    int64_t swapFree {0};
};

class MemoryInfoReader {
    FRIEND_TEST(MemoryInfoReader, read);
public:
    MemoryInfo read() const;
private:
    std::string _procMemInfoFile {"/proc/meminfo"};
};