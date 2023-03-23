#include "MemoryInfoReader.h"

#include <fstream>

MemoryInfo MemoryInfoReader::read() const {
    std::fstream file(_procMemInfoFile, std::ios_base::in);
    if (!file.is_open()) {
        return {};
    }

    MemoryInfo memInfo;

    std::string line;
    while (std::getline(file, line)) {
        auto colonIdx = line.find_first_of(':');

        if (colonIdx != std::string::npos) {
            auto value = std::strtoul(line.c_str() + colonIdx + 1, nullptr, 10) * 1024; // kb -> bytes
            if (line.find("MemTotal") != std::string::npos) {
                memInfo.totalSize = value;
            }
            else if (line.find("MemFree") != std::string::npos) {
                memInfo.free = value;
            }
            else if (line.find("MemAvailable") != std::string::npos) {
                memInfo.available = value;
            }
            else if (line.find("SwapTotal") != std::string::npos) {
                memInfo.swapTotalSize = value;
            }
            else if (line.find("SwapFree") != std::string::npos) {
                memInfo.swapFree = value;
            }
        }
    }

    return memInfo;
}