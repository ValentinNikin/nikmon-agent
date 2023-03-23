#pragma once

#include "extractors/Extractor.h"

class AgentPingExtractor : public Extractor {
public:
    std::unique_ptr<nikmon::api::TaskItem> extract() override;
};
