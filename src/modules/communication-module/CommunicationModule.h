#pragma once

#include <thread>
#include <memory>
#include <atomic>
#include <condition_variable>

#include <Poco/Logger.h>

#include "Client.h"
#include "modules/ThreadedModule.h"
#include "modules/configuration-module/ConfigurationModule.h"
#include "modules/task-manager-module/TaskManagerModule.h"

class CommunicationModule : public ThreadedModule {
public:
    CommunicationModule(
            const std::shared_ptr<ConfigurationModule>& configurationModule,
            const std::shared_ptr<TaskManagerModule>& taskManagerModule);

    bool init();
private:
    void execute() override;
private:
    struct ConfParams {
        std::string serverHost;
        int serverPort;
        int sessionTimeoutMs;
        std::string networkInterface;
    };

    ConfParams _confParams;

    std::shared_ptr<ConfigurationModule> _configurationModule;
private:
    void processRegisterRequest();
    void processStatusRequest();
private:
    std::shared_ptr<TaskManagerModule> _taskManagerModule;

    bool _isRegistered = false;
    std::string _agentId = "";
    std::unique_ptr<Client> _client;

    Poco::Logger& _logger;
};
