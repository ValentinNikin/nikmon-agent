#pragma once

#include <Poco/Util/ServerApplication.h>
#include <Poco/Logger.h>

class Application : public Poco::Util::ServerApplication {
public:
    Application();
protected:
    int main(const std::vector<std::string>&) override;
private:
    bool init();

    Poco::Logger& _logger;
};