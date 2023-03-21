#pragma once

#include <string>

#include "nlohmann-json/json.hpp"

#include "types/enums/CommandType.h"

namespace nikmon {
namespace types {

struct CommandConfirmation {
    std::string taskId;
    CommandType type;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CommandConfirmation, taskId, type)

}
}