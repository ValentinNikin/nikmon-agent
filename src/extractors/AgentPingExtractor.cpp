#include "AgentPingExtractor.h"

std::unique_ptr<nikmon::api::TaskItem> AgentPingExtractor::extract() {
    auto taskItem = std::make_unique<nikmon::api::TaskItem>();
    taskItem->value = "1";
    taskItem->status = TaskResultStatus::Success;
    taskItem->time = std::time(0);
    return taskItem;
}