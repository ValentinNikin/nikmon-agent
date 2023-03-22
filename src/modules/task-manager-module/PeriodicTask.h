#pragma once

#include "Task.h"

class PeriodicTask : public Task {
public:
    PeriodicTask(const std::string& id, std::unique_ptr<Extractor> extractor, const long delay);

    void setDelay(const long delay);
private:
    void threadFunc() override;

    long _delay;

    std::mutex _mutex;
    std::condition_variable _conditionVariable;
};