#include "TaskManagerModule.h"

#include "SingleUseTask.h"
#include "PeriodicTask.h"

TaskManagerModule::TaskManagerModule(const std::shared_ptr<ExtractorFactory>& extractorFactory)
    : _commandsQueue(std::make_unique<CommandsQueue>()),
      _commandConfirmationsQueue(std::make_unique<CommandConfirmationsQueue>()),
      _taskItemsQueue(std::make_unique<TaskItemsQueue>()),
      _extractorsFactory(extractorFactory),
      _logger(Poco::Logger::get("TaskManagerModule")) {}

TaskManagerModule::~TaskManagerModule() {
    stop();

    if (_thread.joinable()) {
        _thread.join();
    }
}

bool TaskManagerModule::start() {
    _thread = std::thread(&TaskManagerModule::threadFunc, this);

    return true;
}

void TaskManagerModule::stop() {
    _isOkToContinue = false;
}

void TaskManagerModule::threadFunc() {
    while (_isOkToContinue) {
        std::unique_lock<std::mutex> lg(_mutex);
        _condVariable.wait_for(lg, std::chrono::milliseconds(_heartbeat));

        if (!_isOkToContinue) {
            return;
        }

        auto commands = _commandsQueue->getAll();
        for (const auto& c : commands) {
            switch (c->type) {
                case CommandType::Add: {
                    auto extractor = _extractorsFactory->buildExtractor(c->payload.key);

                    std::unique_ptr<Task> task;
                    if (c->payload.frequency == TaskFrequency::OnceTime) {
                        task = std::make_unique<SingleUseTask>(c->taskId, std::move(extractor));
                    }
                    else if (c->payload.frequency == TaskFrequency::MultipleTimes) {
                        task = std::make_unique<PeriodicTask>(c->taskId, std::move(extractor), c->payload.delay);
                    }

                    task->start();
                    _tasks.push_back(std::move(task));

                    _commandConfirmationsQueue->insert(nikmon::types::CommandConfirmation(c->taskId, c->type));

                    break;
                }
                case CommandType::Change: {
                    break;
                }
                case CommandType::Cancel: {
                    break;
                }
                default: {
                    _logger.error("Unknown command type: %d", static_cast<int>(c->type));
                }
            }
        }
    }
}

std::vector<nikmon::types::CommandConfirmation> TaskManagerModule::getConfirmations() {
    return _commandConfirmationsQueue->getAll();
}

std::vector<TaskManagerModule::TaskItemPtr> TaskManagerModule::getTaskItems() {
    return _taskItemsQueue->getAll();
}

void TaskManagerModule::addCommands(std::vector<CommandPtr>&& commands) {
    _commandsQueue->insertRange(std::move(commands));
}