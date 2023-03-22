#include "ServiceLocator.h"

#include "modules/communication-module/CommunicationModule.h"
#include "modules/ConfigurationModule.h"
#include "modules/task-manager-module/TaskManagerModule.h"

#include "extractors/ExtractorFactory.h"

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
//    builder.registerType<ExtractorFactory>().singleInstance();

    _container = builder.build();
}