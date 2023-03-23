#include "Task.h"

Task::Task(const std::string& id, const std::string& key, std::unique_ptr<Extractor> extractor)
    : _id(id),
    _key(key),
    _extractor(std::move(extractor)),
    _itemsQueue(std::make_unique<SyncQueue<std::unique_ptr<TaskItem>>>()) {}

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

std::string Task::getId() const {
    return _id;
}

std::string Task::getKey() const {
    return _key;
}

ReadSyncQueue<std::unique_ptr<nikmon::api::TaskItem>>* Task::readyItemsQueue() {
    return _itemsQueue.get();
}

void Task::setExtractor(std::unique_ptr<Extractor> extractor) {
    std::lock_guard<std::mutex> lg(_mutex);
    _extractor = std::move(extractor);
}

void Task::extract() {
    std::unique_ptr<nikmon::api::TaskItem> item;
    {
        std::lock_guard<std::mutex> lg(_mutex);
        item = _extractor->extract();
        item->id = _id;
    }
    _itemsQueue->insert(std::move(item));
}

void Task::setIsFinished() {
    _isFinished = true;
}