#include "StatusRequest.h"

void nikmon::types::to_json(nlohmann::json &j, const StatusRequest &r) {
    j["id"] = r.id;

    auto confirmationsJsonArr = nlohmann::json::array();
    for (const auto& c : r.confirmations) {
        auto confirmationJson = nlohmann::json();
        to_json(confirmationJson, c);
        confirmationsJsonArr.push_back(confirmationJson);
    }
    j["confirmations"] = confirmationsJsonArr;

    auto itemsJsonArr = nlohmann::json::array();
    for (const auto& i : r.items) {
        auto itemJson = nlohmann::json();
        to_json(itemJson, *i);
        itemsJsonArr.push_back(itemJson);
    }
    j["items"] = itemsJsonArr;
}