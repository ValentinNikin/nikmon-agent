#include "Client.h"

#include <sstream>

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/SSLException.h>
#include <Poco/StreamCopier.h>

Client::Client(const std::string& host, const int port, const int sessionTimeoutMs)
    : _host(host), _port(port), _sessionTimeoutMs(sessionTimeoutMs) {}

ServerResponse Client::postRequest(const std::string& uri, const nlohmann::json& requestJson, nlohmann::json& responseJson) {
    if (!_session) {
        _session = std::make_unique<Poco::Net::HTTPClientSession>(_host, _port);
        _session->setTimeout(Poco::Timespan(_sessionTimeoutMs / 1000, 0));
    }

    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, uri);
    request.setContentType("application/json");
    request.setKeepAlive(true);

    auto requestBodyString = nlohmann::to_string(requestJson);

    request.setContentLength(static_cast<std::streamsize>(requestBodyString.size()));

    try {
        _session->sendRequest(request) << requestBodyString;
    }
    catch (Poco::TimeoutException & ex) {
//        LOG_ERROR("HTTP Timeout connection error");
        return ServerResponse::TIMED_OUT;
    } catch (Poco::Net::ConnectionRefusedException & ex) {
//        LOG_ERROR("Connection has been refused");
        return ServerResponse::NETWORK_ERROR;
    } catch (Poco::Net::ConnectionResetException & ex) {
//        LOG_ERROR("Connection has been reset");
        return ServerResponse::NETWORK_ERROR;
    } catch (Poco::Net::ConnectionAbortedException & ex) {
//        LOG_ERROR("Connection has been aborted");
        return ServerResponse::NETWORK_ERROR;
    } catch (Poco::Net::SSLException & ex) {
//        LOG_ERROR("SSL exception. Error msg: %s",ex.message());
        return ServerResponse::NETWORK_ERROR;
    } catch (Poco::IOException & ex) {
//        LOG_ERROR("IO Exception: %s", std::string(ex.what()) );
        return ServerResponse::NETWORK_ERROR;
    } catch (Poco::InvalidArgumentException & ex) {
//        LOG_ERROR("Invalid argument: %s", std::string(ex.what()) );
        return ServerResponse::NETWORK_ERROR;
    }
    catch (...) {
        return ServerResponse::UNKNOWN;
    }

    Poco::Net::HTTPResponse response;
    std::stringstream ss;
    try {
        Poco::StreamCopier::copyStream(_session->receiveResponse(response), ss);
    }
    catch (Poco::Exception &ex) {
//        LOG_ERROR("Unable to receive response from control server: %s, %s",
//                  std::string(ex.what()), ex.displayText());
        return ServerResponse::UNKNOWN;
    }

    auto responseStatus = response.getStatus();
    switch (responseStatus) {
        case 200: {
            responseJson = nlohmann::json::parse(ss.str());
            return ServerResponse::OK;
        }
        case 401:
//            LOG_ERROR("Agent needs authorization: %d", responseStatus);
            return ServerResponse::UNAUTHORIZED;
        case 500:
//            LOG_ERROR("Internal server error: %d", responseStatus);
            return ServerResponse::INTERNAL_SERVER_ERROR;
        default:
//            LOG_ERROR("Unknown status: %d", responseStatus);
            return ServerResponse::UNKNOWN;
    }

    return ServerResponse::UNKNOWN;
}