#include "Task.h"

Task::Task(const std::string& id, std::unique_ptr<Extractor> extractor)
    : _id(id),
    _extractor(std::move(extractor)),
    _itemsQueue(std::make_unique<TaskItemsQueue>()) {}
