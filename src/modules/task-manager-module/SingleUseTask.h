#pragma once

#include "Task.h"

class SingleUseTask : public Task {
public:
    SingleUseTask(const std::string& id, std::unique_ptr<Extractor> extractor);
private:
    void threadFunc() override;
};
