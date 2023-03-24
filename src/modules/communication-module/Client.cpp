#include "Client.h"

#include <sstream>

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/SSLException.h>
#include <Poco/StreamCopier.h>

Client::Client(const std::string& host, const int port, const int sessionTimeoutMs)
    : _host(host), _port(port), _sessionTimeoutMs(sessionTimeoutMs),
      _logger(Poco::Logger::get("Client")) {}

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
        _logger.error("HTTP Timeout connection error");
        return ServerResponse::TIMED_OUT;
    } catch (Poco::Net::ConnectionRefusedException & ex) {
        _logger.error("Connection has been refused");
        return ServerResponse::NETWORK_ERROR;
    } catch (Poco::Net::ConnectionResetException & ex) {
        _logger.error("Connection has been reset");
        return ServerResponse::NETWORK_ERROR;
    } catch (Poco::Net::ConnectionAbortedException & ex) {
        _logger.error("Connection has been aborted");
        return ServerResponse::NETWORK_ERROR;
    } catch (Poco::Net::SSLException & ex) {
        _logger.error("SSL exception. Error msg: %s",ex.message());
        return ServerResponse::NETWORK_ERROR;
    } catch (Poco::IOException & ex) {
        _logger.error("IO Exception: %s", std::string(ex.what()) );
        return ServerResponse::NETWORK_ERROR;
    } catch (Poco::InvalidArgumentException & ex) {
        _logger.error("Invalid argument: %s", std::string(ex.what()) );
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
        _logger.error("Unable to receive response from control server: %s, %s",
                  std::string(ex.what()), ex.displayText());
        return ServerResponse::UNKNOWN;
    }

    auto responseStatus = response.getStatus();
    switch (responseStatus) {
        case 200: {
            responseJson = nlohmann::json::parse(ss.str());
            return ServerResponse::OK;
        }
        case 401:
            _logger.error("Agent needs authorization: %d", static_cast<int>(responseStatus));
            return ServerResponse::UNAUTHORIZED;
        case 500:
            _logger.error("Internal server error: %d", static_cast<int>(responseStatus));
            return ServerResponse::INTERNAL_SERVER_ERROR;
        default:
            _logger.error("Unknown status: %d", static_cast<int>(responseStatus));
            return ServerResponse::UNKNOWN;
    }

    return ServerResponse::UNKNOWN;
}