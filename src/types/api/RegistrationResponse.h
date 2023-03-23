#pragma once

#include <string>

#include "nlohmann-json/json.hpp"

namespace nikmon {
namespace api {

struct RegistrationResponse {
    std::string id;
    int heartbeat;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RegistrationResponse, id, heartbeat)

}
}