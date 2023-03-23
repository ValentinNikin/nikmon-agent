#include "Command.h"

void nikmon::api::to_json(nlohmann::json& j, const Command& c) {
    j["taskId"] = c.taskId;
    j["type"] = c.type;

    if (c.type == CommandType::Add || c.type == CommandType::Change) {
        nlohmann::json payloadJson;
        to_json(payloadJson, c.payload);
        j["payload"] = payloadJson;
    }
}

void nikmon::api::from_json(const nlohmann::json& j, Command& c) {
    j.at("taskId").get_to(c.taskId);
    j.at("type").get_to(c.type);

    if (c.type == CommandType::Add || c.type == CommandType::Change) {
        from_json(j.at("payload"), c.payload);
    }
}