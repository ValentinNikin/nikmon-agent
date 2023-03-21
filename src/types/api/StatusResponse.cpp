#include "StatusResponse.h"

void nikmon::types::from_json(nlohmann::json &j, StatusResponse &response) {
    response.heartbeat = j.at("heartbeat").get<int>();

    for (const auto& cJson : j.at("commands")) {
        auto command = std::make_unique<Command>();
        from_json(cJson, *command);
        response.commands.push_back(std::move(command));
    }
}