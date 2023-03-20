#include "ServiceLocator.h"

#include "modules/communication-module/CommunicationModule.h"
#include "modules/ConfigurationModule.h"

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

    _container = builder.build();
}