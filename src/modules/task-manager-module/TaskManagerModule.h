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

#include "modules/ThreadedModule.h"
#include "extractors/ExtractorFactory.h"
#include "utils/SyncQueue.h"
#include "Task.h"

class TaskManagerModule : public ThreadedModule {
    using Command = nikmon::api::Command;
    using CommandConfirmation = nikmon::api::CommandConfirmation;
    using TaskItem = nikmon::api::TaskItem;
public:
    TaskManagerModule(const std::shared_ptr<ExtractorFactory>& extractorFactory);

    WriteSyncQueue<std::unique_ptr<Command>>* commandsQueue();
    ReadSyncQueue<CommandConfirmation>* commandsConfirmationsQueue();
    ReadSyncQueue<std::unique_ptr<TaskItem>>* taskItemsQueue();

private:
    void execute() override;
private:
    std::unique_ptr<SyncQueue<std::unique_ptr<Command>>> _commandsQueue;
    std::unique_ptr<SyncQueue<CommandConfirmation>> _commandConfirmationsQueue;
    std::unique_ptr<SyncQueue<std::unique_ptr<TaskItem>>> _taskItemsQueue;

    std::list<std::unique_ptr<Task>> _tasks;
    std::shared_ptr<ExtractorFactory> _extractorsFactory;
private:
    Poco::Logger& _logger;
};
