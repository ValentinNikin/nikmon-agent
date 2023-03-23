#pragma once

#include <memory>

#include "Extractor.h"

class ExtractorFactory {
public:
    virtual ~ExtractorFactory() = default;

    virtual std::unique_ptr<Extractor> buildExtractor(const std::string& key);

private:
    virtual std::unique_ptr<Extractor> build_System_MachineInfo_Extractor() = 0;
    virtual std::unique_ptr<Extractor> build_System_Processor_Util_Extractor() = 0;
    virtual std::unique_ptr<Extractor> build_System_Memory_Extractor(const std::string& key) = 0;
    virtual std::unique_ptr<Extractor> build_System_Swap_Extractor(const std::string& key) = 0;
    virtual std::unique_ptr<Extractor> build_Agent_Ping_Extractor();
};
