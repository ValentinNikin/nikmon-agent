#pragma once

#include "extractors/Extractor.h"

enum class SwapSizeType {
    Unknown = 0,
    Usage,
    Free
};

class SystemSwapSizeExtractor : public Extractor {
public:
    SystemSwapSizeExtractor(const SwapSizeType type);

    std::unique_ptr<nikmon::api::TaskItem> extract() override;
private:
    SwapSizeType _type;
};