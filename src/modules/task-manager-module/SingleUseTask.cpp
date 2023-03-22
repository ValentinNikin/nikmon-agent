#include "SingleUseTask.h"

SingleUseTask::SingleUseTask(const std::string& id, std::unique_ptr<Extractor> extractor)
    : Task(id, std::move(extractor)) {}

void SingleUseTask::threadFunc() {
    extract();
    setIsFinished();
}