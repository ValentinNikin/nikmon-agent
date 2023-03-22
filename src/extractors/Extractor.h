#pragma once

#include <memory>

#include "types/api/TaskItem.h"

class Extractor {
public:
    Extractor(const std::string& key);
    virtual ~Extractor() = default;
    virtual std::unique_ptr<nikmon::types::TaskItem> extract() = 0;

    std::string getKey() const;
private:
    std::string _key;
};
