#include "gtest/gtest.h"

#include "Config.h"

#include "extractors/linux/sysinfo/MemoryInfoReader.h"

TEST(MemoryInfoReader, read) {
    MemoryInfoReader memoryInfoReader;
    memoryInfoReader._procMemInfoFile = Config::getInstance()->getResourcesPath() + "/meminfo";
    auto memInfo = memoryInfoReader.read();

    ASSERT_EQ(memInfo.totalSize, 8143917056);
    ASSERT_EQ(memInfo.free, 996352000);
    ASSERT_EQ(memInfo.available, 2856476672);
    ASSERT_EQ(memInfo.swapTotalSize, 1022357504);
    ASSERT_EQ(memInfo.swapFree, 1022357504);
}