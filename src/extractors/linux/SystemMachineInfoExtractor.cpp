#include "SystemMachineInfoExtractor.h"

#include <fstream>
#include <set>
#include <sys/utsname.h>

#include "utils/Utils.h"

#include "sysinfo/MemoryInfoReader.h"

std::string SystemMachineInfoExtractor::extractOsName() {
    std::fstream file(_etsOsReleaseFile, std::ios_base::in);
    if (!file.is_open()) {
        return {};
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.find("PRETTY_NAME") != std::string::npos) {
            auto firstQuoteIdx = line.find_first_of('"');
            auto lastQuoteIdx = line.find_last_of('"');

            if (firstQuoteIdx < lastQuoteIdx) {
                return line.substr(firstQuoteIdx + 1, lastQuoteIdx - firstQuoteIdx - 1);
            }
        }
    }

    return {};
}

std::string SystemMachineInfoExtractor::extractOsArchitecture() {
    utsname buf;

    if (uname(&buf) == -1) {
        return {};
    }

    return std::string(buf.machine);
}

SystemMachineInfoExtractor::CpuInfo SystemMachineInfoExtractor::extractCpuInfo() {
    std::fstream file(_procCpuInfoFile, std::ios_base::in);
    if (!file.is_open()) {
        return {};
    }

    CpuInfo cpuInfo;

    std::set<std::string> sockets;
    std::set<std::string> physicalCores;
    std::set<std::string> logicalCores;

    std::string line;
    while (std::getline(file, line)) {
        if (cpuInfo.name.empty() && line.find("model name") != std::string::npos) {
            auto colonIdx = line.find_first_of(':');

            if (colonIdx != std::string::npos) {
                cpuInfo.name = line.substr(colonIdx + 2);
            }
        }
        else if (cpuInfo.instructions.empty() && line.find("flags") != std::string::npos) {
            auto colonIdx = line.find_first_of(':');

            if (colonIdx != std::string::npos) {
                cpuInfo.instructions = line.substr(colonIdx + 2);
            }
        }
        else if (StringUtils::startWith(line, "physical id")) {
            sockets.insert(line);
        }
        else if (StringUtils::startWith(line, "processor")) {
            logicalCores.insert(line);
        }
        else if (StringUtils::startWith(line, "core id")) {
            physicalCores.insert(line);
        }
    }

    cpuInfo.sockets = static_cast<uint32_t>(sockets.size());
    cpuInfo.physicalCores = static_cast<uint32_t>(physicalCores.size());
    cpuInfo.logicalCores = static_cast<uint32_t>(logicalCores.size());

    return cpuInfo;
}

std::unique_ptr<nikmon::api::TaskItem> SystemMachineInfoExtractor::extract() {
    auto taskItem = std::make_unique<nikmon::api::TaskItem>();
    try {
        nikmon::extractors::MachineInfo info;
        info.osName = extractOsName();
        info.osArchitecture = extractOsArchitecture();

        auto cpuInfo = extractCpuInfo();
        info.cpuName = cpuInfo.name;
        info.cpuSockets = cpuInfo.sockets;
        info.cpuPhysicalCores = cpuInfo.physicalCores;
        info.cpuLogicalCores = cpuInfo.logicalCores;
        info.cpuInstructions = cpuInfo.instructions;

        MemoryInfoReader memoryInfoReader;
        auto memInfo = memoryInfoReader.read();
        info.ramTotalSize = memInfo.totalSize;
        info.swapTotalSize = memInfo.swapTotalSize;

        nlohmann::json infoJson;
        nikmon::extractors::to_json(infoJson, info);

        taskItem->value = to_string(infoJson);
        taskItem->status = TaskResultStatus::Success;
        taskItem->time = std::time(0);
    }
    catch (const std::exception& exception) {
        taskItem->status = TaskResultStatus::Error;
        taskItem->errorMessage = std::string(exception.what());
        taskItem->time = std::time(0);
    }
    return taskItem;
}