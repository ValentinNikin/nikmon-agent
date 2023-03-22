#pragma once

#include <memory>

#include "types/api/TaskItem.h"

class Extractor {
public:
    virtual ~Extractor() = default;
    virtual std::unique_ptr<nikmon::types::TaskItem> extract() = 0;
};
