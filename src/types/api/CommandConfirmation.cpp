#include "CommandConfirmation.h"

nikmon::types::CommandConfirmation::CommandConfirmation(
        const std::string& taskId_, const CommandType type_,
        const bool isConfirmed_, const std::string& errorMessage_)
    : taskId(taskId_), type(type_), isConfirmed(isConfirmed_), errorMessage(errorMessage_) {}