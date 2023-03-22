#include "CommandConfirmation.h"

nikmon::types::CommandConfirmation::CommandConfirmation(const std::string& taskId_, const CommandType type_)
    : taskId(taskId_), type(type_) {}