#include "ExtractorFactory.h"

#include "utils/Utils.h"

#include "AgentPingExtractor.h"

std::unique_ptr<Extractor> ExtractorFactory::buildExtractor(const std::string& key) {
    if (key == "System.MachineInfo") {
        return build_System_MachineInfo_Extractor();
    }
    else if (key == "System.Proc.Util") {
        return build_System_Processor_Util_Extractor();
    }
    else if (StringUtils::startWith(key, "System.Memory.Size")) {
        return build_System_Memory_Extractor(key);
    }
    else if (StringUtils::startWith(key, "System.Swap.Size")) {
        return build_System_Swap_Extractor(key);
    }
    else if (key == "Agent.Ping") {
        return build_Agent_Ping_Extractor();
    }

    return nullptr;
}

std::unique_ptr<Extractor> ExtractorFactory::build_Agent_Ping_Extractor() {
    return std::make_unique<AgentPingExtractor>();
}