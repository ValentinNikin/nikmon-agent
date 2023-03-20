#include "CommunicationModule.h"

#include <nlohmann-json/json.hpp>

#include "types/api/RegistrationRequest.h"
#include "types/api/RegistrationResponse.h"
#include "types/api/StatusRequest.h"
#include "types/api/StatusResponse.h"

using namespace nikmon::types;

#define REGISTRATION_QUERY "/api/agent/registration"
#define STATUS_QUERY "/api/agent/status"

CommunicationModule::CommunicationModule(
    const std::shared_ptr<ConfigurationModule>& configurationModule)
    : _configurationModule(configurationModule),
    _logger(Poco::Logger::get("CommunicationModule")) {}

CommunicationModule::~CommunicationModule() {
    if (_thread.joinable()) {
        _thread.join();
    }
}

void CommunicationModule::threadFunc() {
    while (_isOkToContinue) {
        std::unique_lock<std::mutex> lg(_mutex);
        _condVariable.wait_for(lg, std::chrono::milliseconds(_heartbeat));

        if (!_isOkToContinue) {
            return;
        }

        try {
            if (!_isRegistered) {
                processRegisterRequest();
            } else {
                processStatusRequest();
            }
        }
        catch (const std::exception& exception) {
            // TODO: print something to log
        }
    }
}

bool CommunicationModule::start() {
    // TODO: extract from config parameters for client

    std::string host;
    if (_configurationModule->hasKey("ServerHost")) {
        host = _configurationModule->getString("ServerHost");
    }
    else {
        _logger.error("Unable to extract \"ServerHost\" parameter from config");
        return false;
    }

    int port;
    if (_configurationModule->hasKey("Port")) {
        port = _configurationModule->getInt("Port");
    }
    else {
        _logger.error("Unable to extract \"Port\" parameter from config");
        return false;
    }

    int sessionTimeoutMs = 30000;
    if (_configurationModule->hasKey("SessionTimeoutMs")) {
        sessionTimeoutMs = _configurationModule->getInt("SessionTimeoutMs");
    }
    else {
        _logger.error("Unable to extract \"SessionTimeoutMs\" parameter from config");
        return false;
    }

    _client = std::make_unique<Client>(host, port, sessionTimeoutMs);

    _thread = std::thread(&CommunicationModule::threadFunc, this);

    return true;
}

void CommunicationModule::stop() {
    _isOkToContinue = false;
}

void CommunicationModule::processRegisterRequest() {
    RegistrationRequest request;
    // TODO: extract parameters from system
    request.ip = "";
    request.machineName = "";

    nlohmann::json requestJson;
    nlohmann::json responseJson;

    to_json(requestJson, request);

    auto res = _client->postRequest(REGISTRATION_QUERY, requestJson, responseJson);
    if (res == ServerResponse::OK) {
        RegistrationResponse response;
        from_json(responseJson, response);

        _agentId = response.id;
        _heartbeat = response.heartbeat;
    }
    else {
        // TODO: print something to log
    }
}

void CommunicationModule::processStatusRequest() {
    StatusRequest request;
    request.id = _agentId;
    // TODO: fill other fields

    nlohmann::json requestJson;
    nlohmann::json responseJson;

    to_json(requestJson, request);

    auto res = _client->postRequest(REGISTRATION_QUERY, requestJson, responseJson);
    if (res == ServerResponse::OK) {
        StatusResponse response;
        from_json(responseJson, response);


    }
    else {
        // TODO: print something to log
    }
}