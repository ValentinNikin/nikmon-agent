#pragma once

#include "extractors/Extractor.h"

enum class MemorySizeType {
    Unknown = 0,
    Usage,
    Free,
    Available
};

class SystemMemorySizeExtractor : public Extractor {
public:
    SystemMemorySizeExtractor(const MemorySizeType type);

    std::unique_ptr<nikmon::api::TaskItem> extract() override;
private:
    MemorySizeType _type;
};