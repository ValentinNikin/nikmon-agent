#pragma once

#include <string>

#include "nlohmann-json/json.hpp"

#include "types/enums/TaskResultStatus.h"

namespace nikmon {
namespace api {

struct TaskItem {
    std::string id;
    TaskResultStatus status;
    std::string errorMessage;
    std::string value;
    long time;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TaskItem, id, status, errorMessage, value, time)

}
}