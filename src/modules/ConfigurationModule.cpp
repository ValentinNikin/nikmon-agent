#include "ConfigurationModule.h"

void ConfigurationModule::loadConfig(const std::string& path) {
    _config->load(path);
}