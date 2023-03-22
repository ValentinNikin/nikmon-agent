#include "PeriodicTask.h"

PeriodicTask::PeriodicTask(const std::string& id, std::unique_ptr<Extractor> extractor, const long delay)
    : Task(id, std::move(extractor)),
    _delay(delay) {}

void PeriodicTask::setDelay(const long delay) {
    _delay = delay;
}

void PeriodicTask::threadFunc() {
    while (_isOkToContinue) {
        std::unique_lock<std::mutex> lock(_mutex);
        _conditionVariable.wait_for(lock, std::chrono::milliseconds(_delay));

        if (!_isOkToContinue) {
            break;
        }

        extract();
    }
    setIsFinished();
}