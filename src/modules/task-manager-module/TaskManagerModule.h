#pragma once

#include <thread>
#include <atomic>
#include <mutex>
#include <list>
#include <condition_variable>

#include <Poco/Logger.h>

#include "types/api/Command.h"
#include "types/api/CommandConfirmation.h"
#include "types/api/TaskItem.h"

#include "extractors/ExtractorFactory.h"
#include "utils/SyncQueue.h"
#include "Task.h"

class TaskManagerModule {
    using CommandPtr = std::unique_ptr<nikmon::types::Command>;
    using CommandsQueue = SyncQueue<CommandPtr>;

    using CommandConfirmationsQueue = SyncQueue<nikmon::types::CommandConfirmation>;

    using TaskItemPtr = std::unique_ptr<nikmon::types::TaskItem>;
    using TaskItemsQueue = SyncQueue<TaskItemPtr>;
public:
    TaskManagerModule(const std::shared_ptr<ExtractorFactory>& extractorFactory);
    ~TaskManagerModule();

    bool start();
    void stop();

    std::vector<nikmon::types::CommandConfirmation> getConfirmations();
    std::vector<TaskItemPtr> getTaskItems();
    void addCommands(std::vector<CommandPtr>&& commands);

private:
    std::unique_ptr<CommandsQueue> _commandsQueue;
    std::unique_ptr<CommandConfirmationsQueue> _commandConfirmationsQueue;
    std::unique_ptr<TaskItemsQueue> _taskItemsQueue;
private:
    std::list<std::unique_ptr<Task>> _tasks;
    std::shared_ptr<ExtractorFactory> _extractorsFactory;
private:
    void threadFunc();
    std::thread _thread;

    std::atomic_bool _isOkToContinue;

    int _heartbeat = 1000;
    std::condition_variable _condVariable;
    std::mutex _mutex;

    Poco::Logger& _logger;
};
