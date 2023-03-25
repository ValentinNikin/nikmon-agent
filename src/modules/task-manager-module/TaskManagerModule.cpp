#include "TaskManagerModule.h"

#include "SingleUseTask.h"
#include "PeriodicTask.h"

using Command = nikmon::api::Command;
using CommandConfirmation = nikmon::api::CommandConfirmation;
using TaskItem = nikmon::api::TaskItem;

TaskManagerModule::TaskManagerModule(const std::shared_ptr<ExtractorFactory>& extractorFactory)
    : _commandsQueue(std::make_unique<SyncQueue<std::unique_ptr<Command>>>()),
      _commandConfirmationsQueue(std::make_unique<SyncQueue<CommandConfirmation>>()),
      _taskItemsQueue(std::make_unique<SyncQueue<std::unique_ptr<TaskItem>>>()),
      _extractorsFactory(extractorFactory),
      _logger(Poco::Logger::get("TaskManagerModule")) {
    changeHeartbeat(5000);
}

void TaskManagerModule::execute() {
    // process commands from server
    auto commands = _commandsQueue->getAll();
    for (const auto& c : commands) {
        nikmon::api::CommandConfirmation confirmation;
        switch (c->type) {
            case CommandType::Add: {
                auto extractor = _extractorsFactory->buildExtractor(c->payload.key);
                if (extractor == nullptr) {
                    _logger.warning("Unable to apply 'Add' command. Unable to create extractor for key: %s", c->payload.key);
                    confirmation = nikmon::api::CommandConfirmation(
                            c->taskId, c->type, false, "Unable to create extractor");
                    break;
                }

                std::unique_ptr<Task> task;
                if (c->payload.frequency == TaskFrequency::OnceTime) {
                    task = std::make_unique<SingleUseTask>(c->taskId, c->payload.key, std::move(extractor));
                }
                else if (c->payload.frequency == TaskFrequency::MultipleTimes) {
                    task = std::make_unique<PeriodicTask>(c->taskId, c->payload.key, std::move(extractor), c->payload.delay);
                }

                task->start();
                _tasks.push_back(std::move(task));

                confirmation = nikmon::api::CommandConfirmation(c->taskId, c->type);

                break;
            }
            case CommandType::Change: {
                auto taskIt = std::find_if(_tasks.begin(), _tasks.end(), [&c](const auto& task) {
                    return task->getId() == c->taskId;
                });

                if (taskIt != _tasks.end()) {
                    if (auto periodicTask = dynamic_cast<PeriodicTask*>(taskIt->get())) {
                        if (c->payload.key != periodicTask->getKey()) {
                            periodicTask->setExtractor(_extractorsFactory->buildExtractor(c->payload.key));
                        }
                        periodicTask->setDelay(c->payload.delay);
                        confirmation = nikmon::api::CommandConfirmation(c->taskId, c->type);
                    }
                    else {
                        _logger.warning("Unable to apply 'Change' command. 'Change' command supporting only by 'MultipleTimes' tasks");
                        confirmation = nikmon::api::CommandConfirmation(
                                c->taskId, c->type, false, "'Change' command supporting only by 'MultipleTimes' tasks");
                    }
                }
                else {
                    _logger.warning("Unable to apply 'Change' command. Task with id " + c->taskId + " not found");
                    confirmation = nikmon::api::CommandConfirmation(
                            c->taskId, c->type, false, "Task with id " + c->taskId + " not found");
                }

                break;
            }
            case CommandType::Cancel: {
                auto taskIt = std::find_if(_tasks.begin(), _tasks.end(), [&c](const auto& task) {
                    return task->getId() == c->taskId;
                });

                if (taskIt != _tasks.end()) {
                    taskIt->get()->stop();
                    _tasks.erase(taskIt);
                    confirmation = nikmon::api::CommandConfirmation(c->taskId, c->type);
                }
                else {
                    _logger.warning("Unable to apply 'Cancel' command. Task with id " + c->taskId + " not found");
                    confirmation = nikmon::api::CommandConfirmation(
                            c->taskId, c->type, false, "Task with id " + c->taskId + " not found");
                }

                break;
            }
            default: {
                confirmation = nikmon::api::CommandConfirmation(
                        c->taskId, c->type, false, "Unknown command type");
                _logger.error("Unknown command type: %d", static_cast<int>(c->type));
            }
        }

        _commandConfirmationsQueue->insert(std::move(confirmation));
    }

    for (auto it = _tasks.begin(); it != _tasks.end(); it++) {
        // collect items from task
        _taskItemsQueue->insertRange(it->get()->readyItemsQueue()->getAll());

        // remove task from list if it is finished
        if (it->get()->isFinished()) {
            it = _tasks.erase(it);
        }
    }
}

WriteSyncQueue<std::unique_ptr<Command>>* TaskManagerModule::commandsQueue() {
    return _commandsQueue.get();
}

ReadSyncQueue<CommandConfirmation>* TaskManagerModule::commandsConfirmationsQueue() {
    return _commandConfirmationsQueue.get();
}

ReadSyncQueue<std::unique_ptr<TaskItem>>* TaskManagerModule::taskItemsQueue() {
    return _taskItemsQueue.get();
}