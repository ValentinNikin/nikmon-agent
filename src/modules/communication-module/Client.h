#pragma once

#include <string>
#include <memory>

#include <nlohmann-json/json.hpp>

#include <Poco/Net/HTTPClientSession.h>

enum class ServerResponse {
    OK = 0,
    TIMED_OUT,
    NETWORK_ERROR,
    UNAUTHORIZED,
    INTERNAL_SERVER_ERROR,
    UNKNOWN
};

class Client {
public:
    Client(const std::string& host, const int port, const int sessionTimeoutMs);

    ServerResponse postRequest(const std::string& uri, const nlohmann::json& requestJson, nlohmann::json& responseJson);
private:
    std::string _host;
    int _port;

    std::unique_ptr<Poco::Net::HTTPClientSession> _session;
    int _sessionTimeoutMs;
};