#include "Task.h"

Task::Task(const std::string& id, std::unique_ptr<Extractor> extractor)
    : _id(id),
    _extractor(std::move(extractor)),
    _itemsQueue(std::make_unique<TaskItemsQueue>()) {}

Task::~Task() {
    stop();

    if (_thread.joinable()) {
        _thread.join();
    }
}

void Task::start() {
    _thread = std::thread(&Task::threadFunc, this);
}

void Task::stop() {
    _isOkToContinue = false;
}

bool Task::isFinished() const {
    return _isFinished;
}

Task::TaskItemsQueue* Task::readyItemsQueue() {
    return _itemsQueue.get();
}

void Task::setExtractor(std::unique_ptr<Extractor> extractor) {
    std::lock_guard<std::mutex> lg(_mutex);
    _extractor = std::move(extractor);
}

void Task::extract() {
    std::unique_ptr<nikmon::types::TaskItem> item;
    {
        std::lock_guard<std::mutex> lg(_mutex);
        item = _extractor->extract();
    }
    _itemsQueue->insert(std::move(item));
}

void Task::setIsFinished() {
    _isFinished = true;
}