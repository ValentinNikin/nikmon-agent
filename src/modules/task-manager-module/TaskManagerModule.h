#pragma once

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "types/api/Command.h"
#include "types/api/CommandConfirmation.h"
#include "types/api/TaskItem.h"

#include "utils/SyncQueue.h"

class TaskManagerModule {
    using CommandPtr = std::unique_ptr<nikmon::types::Command>;
    using CommandsQueue = SyncQueue<CommandPtr>;

    using CommandConfirmationsQueue = SyncQueue<nikmon::types::CommandConfirmation>;

    using TaskItemPtr = std::unique_ptr<nikmon::types::TaskItem>;
    using TaskItemsQueue = SyncQueue<TaskItemPtr>;
public:
    TaskManagerModule();
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
    void threadFunc();
    std::thread _thread;

    std::atomic_bool _isOkToContinue;

    int _heartbeat = 1000;
    std::condition_variable _condVariable;
    std::mutex _mutex;
};
