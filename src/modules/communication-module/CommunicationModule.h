#pragma once

#include <thread>
#include <memory>
#include <atomic>
#include <condition_variable>

#include <Poco/Logger.h>

#include "Client.h"
#include "modules/ConfigurationModule.h"

class CommunicationModule {
public:
    CommunicationModule(const std::shared_ptr<ConfigurationModule>& configurationModule);
    ~CommunicationModule();

    bool start();
    void stop();
private:

    std::shared_ptr<ConfigurationModule> _configurationModule;

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
