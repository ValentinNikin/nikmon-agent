#pragma once

#include <thread>
#include <memory>
#include <atomic>
#include <condition_variable>

#include <Poco/Logger.h>

#include "Client.h"
#include "modules/ConfigurationModule.h"
#include "modules/task-manager-module/TaskManagerModule.h"

class CommunicationModule {
public:
    CommunicationModule(
            const std::shared_ptr<ConfigurationModule>& configurationModule,
            const std::shared_ptr<TaskManagerModule>& taskManagerModule);
    ~CommunicationModule();

    bool start();
    void stop();
private:

    struct ConfParams {
        std::string serverHost;
        int serverPort;
        int sessionTimeoutMs;
        std::string networkInterface;
    };

    ConfParams _confParams;

    bool init();

    std::shared_ptr<ConfigurationModule> _configurationModule;
    std::shared_ptr<TaskManagerModule> _taskManagerModule;

    void processRegisterRequest();
    void processStatusRequest();
    bool _isRegistered = false;
    std::string _agentId = "";



    void threadFunc();
    std::thread _thread;

    std::atomic_bool _isOkToContinue;

    int _heartbeat;
    std::condition_variable _condVariable;
    std::mutex _mutex;

    std::unique_ptr<Client> _client;

    Poco::Logger& _logger;
};
