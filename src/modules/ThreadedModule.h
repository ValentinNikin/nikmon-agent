#pragma once

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

class ThreadedModule {
public:
    ThreadedModule() = default;
    virtual ~ThreadedModule();

    void start();
    void stop();

protected:
    void changeHeartbeat(const int heartbeat);
private:
    virtual void execute() = 0;
private:
    void threadFunc();
    std::thread _thread;

    std::atomic_bool _isOkToContinue;

    int _heartbeat = 1000;
    std::condition_variable _condVariable;
    std::mutex _mutex;
};