#include "ConfigurationModule.h"

ConfigurationModule::ConfigurationModule()
    : _config(new Poco::Util::PropertyFileConfiguration()) {}

void ConfigurationModule::loadConfig(const std::string& path) {
    _config->load(path);
}

bool ConfigurationModule::hasKey(const std::string& key) {
    return _config->has(key);
}

int ConfigurationModule::getInt(const std::string& key) {
    return _config->getInt(key);
}

std::string ConfigurationModule::getString(const std::string& key) {
    return _config->getString(key);
}