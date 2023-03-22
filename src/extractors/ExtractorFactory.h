#pragma once

#include <memory>

#include "Extractor.h"

class ExtractorFactory {
public:
    virtual ~ExtractorFactory() = default;

    virtual std::unique_ptr<Extractor> buildExtractor(const std::string& key) = 0;
};
