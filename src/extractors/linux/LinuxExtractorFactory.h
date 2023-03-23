#pragma once

#include "extractors/ExtractorFactory.h"

class LinuxExtractorFactory : public ExtractorFactory {
public:
    std::unique_ptr<Extractor> buildExtractor(const std::string& key) override;
private:
    std::unique_ptr<Extractor> build_System_MachineInfo_Extractor() override;
    std::unique_ptr<Extractor> build_System_Processor_Util_Extractor() override;
    std::unique_ptr<Extractor> build_System_Memory_Extractor(const std::string& key) override;
    std::unique_ptr<Extractor> build_System_Swap_Extractor(const std::string& key) override;
};