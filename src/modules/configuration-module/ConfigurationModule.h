#pragma once

#include <memory>

#include <Poco/Util/PropertyFileConfiguration.h>

class ConfigurationModule {
public:
    ConfigurationModule() = default;

    void loadConfig(const std::string& path);

    bool hasKey(const std::string& key);
    int getInt(const std::string& key);
    std::string getString(const std::string& key);

private:
    std::shared_ptr<Poco::Util::PropertyFileConfiguration> _config;
};