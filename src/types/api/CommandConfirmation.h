#pragma once

#include <string>

#include "nlohmann-json/json.hpp"

#include "types/enums/CommandType.h"

namespace nikmon {
namespace api {

struct CommandConfirmation {
    std::string taskId;
    CommandType type;
    bool isConfirmed;
    std::string errorMessage;

    CommandConfirmation() = default;
    CommandConfirmation(
            const std::string& taskId_, const CommandType type_,
            const bool isConfirmed_ = true, const std::string& errorMessage_ = "");
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CommandConfirmation, taskId, type, isConfirmed, errorMessage)

}
}