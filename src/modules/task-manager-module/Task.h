#pragma once

#include <string>
#include <memory>

#include "extractors/Extractor.h"

#include "utils/SyncQueue.h"
#include "types/api/TaskItem.h"

class Task {
    using TaskItemsQueue = SyncQueue<std::unique_ptr<nikmon::types::TaskItem>>;
public:
    Task(const std::string& id, std::unique_ptr<Extractor> extractor);

    virtual ~Task() = default;

    void run();
    void stop();

    TaskItemsQueue* readyItemsQueue();

private:
    std::string _id;
    std::unique_ptr<Extractor> _extractor;

    std::unique_ptr<TaskItemsQueue> _itemsQueue;
};