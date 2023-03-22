#include "ThreadedModule.h"

ThreadedModule::~ThreadedModule() {
    stop();

    if (_thread.joinable()) {
        _thread.join();
    }
}

void ThreadedModule::start() {
    _thread = std::thread(&ThreadedModule::threadFunc, this);
}

void ThreadedModule::stop() {
    _isOkToContinue = false;
    _condVariable.notify_one();
}

void ThreadedModule::threadFunc() {
    while (_isOkToContinue) {
        std::unique_lock<std::mutex> lg(_mutex);
        std::cv_status status;
        do {
            status = _condVariable.wait_for(lg, std::chrono::milliseconds(_heartbeat));
        }
        while (status != std::cv_status::timeout && _isOkToContinue);


        if (!_isOkToContinue) {
            return;
        }

        execute();
    }
}

void ThreadedModule::changeHeartbeat(const int heartbeat) {
    _heartbeat = heartbeat;
}