#include "TaskManagerModule.h"

TaskManagerModule::TaskManagerModule()
    : _commandsQueue(std::make_unique<CommandsQueue>()),
      _commandConfirmationsQueue(std::make_unique<CommandConfirmationsQueue>()),
      _taskItemsQueue(std::make_unique<TaskItemsQueue>()) {}

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