#include "CommunicationModule.h"

#include <Poco/Environment.h>
#include <Poco/Net/IPAddress.h>
#include <Poco/Net/NetworkInterface.h>

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
            _logger.error("An exception occurred while executing a request to the server, what: %s", std::string(exception.what()));
        }
    }
}

bool CommunicationModule::start() {
    if (!init()) {
        return false;
    }

    _client = std::make_unique<Client>(_confParams.serverHost, _confParams.serverPort, _confParams.sessionTimeoutMs);

    _thread = std::thread(&CommunicationModule::threadFunc, this);

    return true;
}

void CommunicationModule::stop() {
    _isOkToContinue = false;
}

bool CommunicationModule::init() {
    if (_configurationModule->hasKey("ServerHost")) {
        _confParams.serverHost = _configurationModule->getString("ServerHost");
    }
    else {
        _logger.error("Unable to extract \"ServerHost\" parameter from config");
        return false;
    }

    if (_configurationModule->hasKey("Port")) {
        _confParams.serverPort = _configurationModule->getInt("Port");
    }
    else {
        _logger.error("Unable to extract \"Port\" parameter from config");
        return false;
    }

    if (_configurationModule->hasKey("SessionTimeoutMs")) {
        _confParams.sessionTimeoutMs = _configurationModule->getInt("SessionTimeoutMs");
    }
    else {
        _logger.error("Unable to extract \"SessionTimeoutMs\" parameter from config");
        return false;
    }

    if (_configurationModule->hasKey("NetworkInterface")) {
        _confParams.networkInterface = _configurationModule->getInt("NetworkInterface");
    }
    else {
        _logger.error("Unable to extract \"NetworkInterface\" parameter from config");
        return false;
    }

    return true;
}

namespace {
    bool extractLocalIp(Poco::Net::IPAddress& ipAddress, const std::string& interface) {
        auto interfaces = Poco::Net::NetworkInterface::list(1, 1);

        for (const auto& i : interfaces) {
            if (i.displayName() == interface && i.address().family() == Poco::Net::IPAddress::Family::IPv4 && !i.isLoopback()) {
                ipAddress = i.address();
                return true;
            }
        }

        return false;
    }
}

void CommunicationModule::processRegisterRequest() {
    Poco::Net::IPAddress ipAddress;
    if (!extractLocalIp(ipAddress, _confParams.networkInterface)) {
        _logger.error("Unable to extract local IP Address");
        return;
    }

    RegistrationRequest request;
    request.ip = ipAddress.toString();
    request.machineName = Poco::Environment::nodeName();

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
        _logger.error("Unable to execute registration request");
    }

    _isRegistered = res == ServerResponse::OK;
}

void CommunicationModule::processStatusRequest() {
    StatusRequest request;
    request.id = _agentId;
    // TODO: fill other fields

    nlohmann::json requestJson;
    nlohmann::json responseJson;

    to_json(requestJson, request);

    auto res = _client->postRequest(STATUS_QUERY, requestJson, responseJson);
    if (res == ServerResponse::OK) {
        StatusResponse response;
        from_json(responseJson, response);

        // TODO: process response
    }
    else {
        _logger.error("Unable to execute status request");
    }
}