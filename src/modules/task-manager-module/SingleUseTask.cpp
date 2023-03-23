#include "SingleUseTask.h"

SingleUseTask::SingleUseTask(const std::string& id, const std::string& key, std::unique_ptr<Extractor> extractor)
    : Task(id, key, std::move(extractor)) {}

void SingleUseTask::threadFunc() {
    extract();
    setIsFinished();
}