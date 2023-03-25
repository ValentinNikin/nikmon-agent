#include "SystemMemorySizeExtractor.h"

#include "sysinfo/MemoryInfoReader.h"

SystemMemorySizeExtractor::SystemMemorySizeExtractor(const MemorySizeType type)
    : _type(type) {}

std::unique_ptr<nikmon::api::TaskItem> SystemMemorySizeExtractor::extract() {
    auto taskItem = std::make_unique<nikmon::api::TaskItem>();
    try {
        MemoryInfoReader memoryInfoReader;
        auto memInfo = memoryInfoReader.read();

        taskItem->status = TaskResultStatus::Success;
        taskItem->time = std::time(0);

        if (_type == MemorySizeType::Free) {
            taskItem->value = std::to_string(memInfo.free);
        }
        else if (_type == MemorySizeType::Available) {
            taskItem->value = std::to_string(memInfo.available);
        }
        else if (_type == MemorySizeType::Usage) {
            taskItem->value = std::to_string(memInfo.totalSize - memInfo.available);
        }
    }
    catch (const std::exception& exception) {
        taskItem->status = TaskResultStatus::Error;
        taskItem->errorMessage = std::string(exception.what());
        taskItem->time = std::time(0);
    }

    return taskItem;
}