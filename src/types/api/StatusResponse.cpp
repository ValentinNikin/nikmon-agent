#include "StatusResponse.h"

void nikmon::types::to_json(nlohmann::json& j, const StatusResponse& response) {
    j = nlohmann::json();
    j["heartbeat"] = response.heartbeat;
    j["commands"] = nlohmann::json::array();
    for (const auto& c : response.commands) {
        nlohmann::json commandJson;
        to_json(commandJson, *c);
        j["commands"].push_back(commandJson);
    }
}

void nikmon::types::from_json(nlohmann::json &j, StatusResponse &response) {
    response.heartbeat = j.at("heartbeat").get<int>();

    for (const auto& cJson : j.at("commands")) {
        auto command = std::make_unique<Command>();
        from_json(cJson, *command);
        response.commands.push_back(std::move(command));
    }
}