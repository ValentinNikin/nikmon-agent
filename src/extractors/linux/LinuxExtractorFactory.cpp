#include "LinuxExtractorFactory.h"

std::unique_ptr<Extractor> LinuxExtractorFactory::buildExtractor(const std::string& key) {
    if (auto extractor = ExtractorFactory::buildExtractor(key)) {
        return extractor;
    }

    // TODO: Here you can add logic to build extractor specific for linux
    return nullptr;
}

std::unique_ptr<Extractor> LinuxExtractorFactory::build_System_MachineInfo_Extractor() {
    return nullptr;
}

std::unique_ptr<Extractor> LinuxExtractorFactory::build_System_Processor_Util_Extractor() {
    return nullptr;
}

std::unique_ptr<Extractor> LinuxExtractorFactory::build_System_Memory_Usage_Extractor() {
    return nullptr;
}

std::unique_ptr<Extractor> LinuxExtractorFactory::build_System_Swap_Usage_Extractor() {
    return nullptr;
}

std::unique_ptr<Extractor> LinuxExtractorFactory::build_Agent_Ping_Extractor() {
    return nullptr;
}