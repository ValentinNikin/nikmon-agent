#pragma once

#include <string>
#include <memory>
#include <thread>
#include <condition_variable>
#include <atomic>

#include "extractors/Extractor.h"

#include "utils/SyncQueue.h"
#include "types/api/TaskItem.h"
#include "types/enums/TaskFrequency.h"

class Task {
    using TaskItem = nikmon::types::TaskItem;
public:
    Task(const std::string& id, std::unique_ptr<Extractor> extractor);
    virtual ~Task();

    void start();
    void stop();

    bool isFinished() const;
    std::string getId() const;
    std::string getKey() const;

    ReadSyncQueue<std::unique_ptr<TaskItem>>* readyItemsQueue();

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

    std::unique_ptr<SyncQueue<std::unique_ptr<TaskItem>>> _itemsQueue;

    std::atomic_bool _isFinished = false;
    TaskFrequency _frequency;
};