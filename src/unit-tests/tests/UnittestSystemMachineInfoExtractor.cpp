#include "gtest/gtest.h"

#include "Config.h"

#include "extractors/linux/SystemMachineInfoExtractor.h"

TEST(SystemMachineInfoExtractor, extractOsName) {
    SystemMachineInfoExtractor extractor;
    extractor._etsOsReleaseFile = Config::getInstance()->getResourcesPath() + "/os-release";

    ASSERT_EQ(extractor.extractOsName(), "Debian GNU/Linux 11 (bullseye)");
}

TEST(SystemMachineInfoExtractor, extractCpuInfo) {
    SystemMachineInfoExtractor extractor;
    extractor._procCpuInfoFile = Config::getInstance()->getResourcesPath() + "/cpuinfo";

    auto cpuInfo = extractor.extractCpuInfo();

    ASSERT_EQ(cpuInfo.name, "AMD Ryzen 7 3700X 8-Core Processor");
    ASSERT_EQ(cpuInfo.instructions, "fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 ht syscall nx mmxext fxsr_opt rdtscp lm constant_tsc rep_good nopl nonstop_tsc cpuid extd_apicid tsc_known_freq pni pclmulqdq ssse3 cx16 sse4_1 sse4_2 x2apic movbe popcnt aes xsave avx rdrand hypervisor lahf_lm cmp_legacy cr8_legacy abm sse4a misalignsse 3dnowprefetch ssbd vmmcall fsgsbase avx2 rdseed clflushopt arat");
    ASSERT_EQ(cpuInfo.physicalCores, 6);
    ASSERT_EQ(cpuInfo.logicalCores, 6);
    ASSERT_EQ(cpuInfo.sockets, 1);
}