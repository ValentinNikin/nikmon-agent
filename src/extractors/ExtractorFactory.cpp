#include "ExtractorFactory.h"

std::unique_ptr<Extractor> ExtractorFactory::buildExtractor(const std::string& key) {
    if (key == "System.MachineInfo") {
        return build_System_MachineInfo_Extractor();
    }
    else if (key == "System.Proc.Util") {
        return build_System_Processor_Util_Extractor();
    }
    else if (key == "System.Memory.Usage") {
        return build_System_Memory_Usage_Extractor();
    }
    else if (key == "System.Swap.Usage") {
        return build_System_Swap_Usage_Extractor();
    }
    else if (key == "Agent.Ping") {
        return build_Agent_Ping_Extractor();
    }

    return nullptr;
}