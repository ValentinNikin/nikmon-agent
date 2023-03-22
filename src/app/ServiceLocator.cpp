#include "ServiceLocator.h"

#include "modules/communication-module/CommunicationModule.h"
#include "modules/configuration-module/ConfigurationModule.h"
#include "modules/task-manager-module/TaskManagerModule.h"

#include "extractors/ExtractorFactory.h"
#include "extractors/linux/LinuxExtractorFactory.h"

std::unique_ptr<ServiceLocator> ServiceLocator::_instance = nullptr;

ServiceLocator* ServiceLocator::getInstance() {
    if (_instance == nullptr) {
        _instance = std::unique_ptr<ServiceLocator>(new ServiceLocator());
        _instance->init();
    }

    return _instance.get();
}

void ServiceLocator::init() {
    Hypodermic::ContainerBuilder builder;

    builder.registerType<CommunicationModule>().singleInstance();
    builder.registerType<ConfigurationModule>().singleInstance();
    builder.registerType<TaskManagerModule>().singleInstance();

    // TODO: write some logic to check current OS and select right factory
    builder.registerType<LinuxExtractorFactory>()
            .as<ExtractorFactory>().singleInstance();

    _container = builder.build();
}