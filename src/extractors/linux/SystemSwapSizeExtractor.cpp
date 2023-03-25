#include "SystemSwapSizeExtractor.h"

#include "sysinfo/MemoryInfoReader.h"

SystemSwapSizeExtractor::SystemSwapSizeExtractor(const SwapSizeType type)
        : _type(type) {}

std::unique_ptr<nikmon::api::TaskItem> SystemSwapSizeExtractor::extract() {
    auto taskItem = std::make_unique<nikmon::api::TaskItem>();
    try {
        MemoryInfoReader memoryInfoReader;
        auto memInfo = memoryInfoReader.read();

        taskItem->status = TaskResultStatus::Success;
        taskItem->time = std::time(0);

        if (_type == SwapSizeType::Free) {
            taskItem->value = std::to_string(memInfo.swapFree);
        }
        else if (_type == SwapSizeType::Usage) {
            taskItem->value = std::to_string(memInfo.swapTotalSize - memInfo.swapFree);
        }
    }
    catch (const std::exception& exception) {
        taskItem->status = TaskResultStatus::Error;
        taskItem->errorMessage = std::string(exception.what());
        taskItem->time = std::time(0);
    }

    return taskItem;
}