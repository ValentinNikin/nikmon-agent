#include "Application.h"

#include <Poco/Util/LoggingConfigurator.h>

#include "app/ServiceLocator.h"
#include "modules/ConfigurationModule.h"

namespace {
    Poco::Path getProjectConfigurationsPath() {
        return Poco::Path().home().append(".config").append("nikmon-agent");
    }

    std::string getConfigPath() {
        return getProjectConfigurationsPath().append("conf").append("agent.config").toString();
    }

    std::string getLogConfigPath() {
        return getProjectConfigurationsPath().append("conf").append("logger.properties").toString();
    }
}

Application::Application()
    : _logger(Poco::Logger::get("Application")) {}

int Application::main(const std::vector<std::string>&) {
    if (!init()) {
        return Poco::Util::ServerApplication::EXIT_SOFTWARE;
    }

    waitForTerminationRequest();

    return Poco::Util::ServerApplication::EXIT_OK;
}

bool Application::init() {
    try {
        _logger.debug("Reading logger configuration...");
        Poco::AutoPtr<Poco::Util::PropertyFileConfiguration> logConfig(
                new Poco::Util::PropertyFileConfiguration(getLogConfigPath()));
        Poco::Util::LoggingConfigurator logConfigurator;
        logConfigurator.configure(logConfig);
        _logger.information("Logger configuration successfully read");
    } catch (const std::exception& exception) {
        _logger.fatal("Unable to configure logger: %s", exception.what());
        return false;
    }

    try {
        _logger.debug("Reading configuration file...");
        auto configurationModule = ServiceLocator::getInstance()->resolve<ConfigurationModule>();
        configurationModule->loadConfig(getConfigPath());
        _logger.information("Application configuration file successfully read");
    }
    catch (const std::exception& exception) {
        _logger.fatal("Unable to read application configuration file: %s", exception.what());
        return false;
    }

    return true;
}