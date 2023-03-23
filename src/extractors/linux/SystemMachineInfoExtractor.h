#pragma once

#include "gtest/gtest.h"

#include "extractors/Extractor.h"

#include <nlohmann-json/json.hpp>

namespace nikmon {
namespace extractors {

struct MachineInfo {
    std::string osName;
    std::string osArchitecture;
    std::string cpuName;
    uint32_t cpuSockets;
    uint32_t cpuPhysicalCores;
    uint32_t cpuLogicalCores;
    std::string cpuInstructions;
    int64_t ramTotalSize;
    int64_t swapTotalSize;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
        MachineInfo,
        osName, osArchitecture, cpuName, cpuSockets, cpuPhysicalCores,
        cpuLogicalCores, cpuInstructions, ramTotalSize, swapTotalSize)
}
}


class SystemMachineInfoExtractor : public Extractor {
    FRIEND_TEST(SystemMachineInfoExtractor, extractOsName);
    FRIEND_TEST(SystemMachineInfoExtractor, extractCpuInfo);
public:
    std::unique_ptr<nikmon::api::TaskItem> extract() override;

private:
    struct CpuInfo {
        std::string name;
        uint32_t sockets {0};
        uint32_t physicalCores {0};
        uint32_t logicalCores {0};
        std::string instructions;
    };

    std::string extractOsName();
    std::string extractOsArchitecture();
    CpuInfo extractCpuInfo();

private:
    // backdoor for the unit-test to substitute path and check functions
    std::string _etsOsReleaseFile {"/etc/os-release"};
    std::string _procCpuInfoFile {"/proc/cpuinfo"};
};
