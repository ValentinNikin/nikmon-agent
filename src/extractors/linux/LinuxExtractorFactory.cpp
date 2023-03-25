#include "LinuxExtractorFactory.h"

#include "SystemMachineInfoExtractor.h"
#include "SystemMemorySizeExtractor.h"
#include "SystemSwapSizeExtractor.h"

std::unique_ptr<Extractor> LinuxExtractorFactory::buildExtractor(const std::string& key) {
    if (auto extractor = ExtractorFactory::buildExtractor(key)) {
        return extractor;
    }

    // TODO: Here you can add logic to build extractor specific for linux
    return nullptr;
}

std::unique_ptr<Extractor> LinuxExtractorFactory::build_System_MachineInfo_Extractor() {
    return std::make_unique<SystemMachineInfoExtractor>();
}

std::unique_ptr<Extractor> LinuxExtractorFactory::build_System_Processor_Util_Extractor() {
    return nullptr;
}

std::unique_ptr<Extractor> LinuxExtractorFactory::build_System_Memory_Extractor(const std::string& key) {
    auto openBracketIdx = key.find_first_of('[');
    auto closeBracketIdx = key.find_first_of(']');

    if (openBracketIdx != std::string::npos && closeBracketIdx != std::string::npos && openBracketIdx < closeBracketIdx) {
        auto typeStr = key.substr(openBracketIdx + 1, closeBracketIdx - openBracketIdx - 1);
        MemorySizeType type {MemorySizeType::Unknown};
        if (typeStr == "Usage") {
            type = MemorySizeType::Usage;
        }
        else if (typeStr == "Free") {
            type = MemorySizeType::Free;
        }
        else if (typeStr == "Available") {
            type = MemorySizeType::Available;
        }

        if (type != MemorySizeType::Unknown) {
            return std::make_unique<SystemMemorySizeExtractor>(type);
        }

        // TODO: print something to log about unable to parse key
    }
    return nullptr;
}

std::unique_ptr<Extractor> LinuxExtractorFactory::build_System_Swap_Extractor(const std::string& key) {
    auto openBracketIdx = key.find_first_of('[');
    auto closeBracketIdx = key.find_first_of(']');

    if (openBracketIdx != std::string::npos && closeBracketIdx != std::string::npos && openBracketIdx < closeBracketIdx) {
        auto typeStr = key.substr(openBracketIdx + 1, closeBracketIdx - openBracketIdx - 1);
        SwapSizeType type {SwapSizeType::Unknown};
        if (typeStr == "Usage") {
            type = SwapSizeType::Usage;
        }
        else if (typeStr == "Free") {
            type = SwapSizeType::Free;
        }

        if (type != SwapSizeType::Unknown) {
            return std::make_unique<SystemSwapSizeExtractor>(type);
        }

        // TODO: print something to log about unable to parse key
    }
    return nullptr;
}