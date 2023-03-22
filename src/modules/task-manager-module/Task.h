#pragma once

#include <string>
#include <memory>
#include <thread>
#include <condition_variable>
#include <atomic>

#include "extractors/Extractor.h"

#include "utils/SyncQueue.h"
#include "types/api/TaskItem.h"

class Task {
    using TaskItemsQueue = SyncQueue<std::unique_ptr<nikmon::types::TaskItem>>;
public:
    Task(const std::string& id, std::unique_ptr<Extractor> extractor);
    virtual ~Task();

    void start();
    void stop();

    bool isFinished() const;

    TaskItemsQueue* readyItemsQueue();

    void setExtractor(std::unique_ptr<Extractor> extractor);

protected:
    void extract();
    void setIsFinished();

    std::atomic_bool _isOkToContinue = true;
private:
    virtual void threadFunc() = 0;
    std::thread _thread;

    std::string _id;
    std::unique_ptr<Extractor> _extractor;
    std::mutex _mutex;

    std::unique_ptr<TaskItemsQueue> _itemsQueue;

    std::atomic_bool _isFinished = false;
};