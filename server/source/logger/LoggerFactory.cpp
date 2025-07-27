#include "LoggerFactory.h"

#include "P7Logger.h"

namespace server {
    std::shared_ptr<ILogger> LoggerFactory::createLogger(std::string const &name, std::string const &log_dir, std::string const& log_level) {
        return std::make_shared<P7Logger>(name, log_dir, log_level);
    }

    std::shared_ptr<ILogger> LoggerFactory::getSingletonLogger() {
        static std::shared_ptr<ILogger> instance = std::make_shared<P7Logger>("Main", "CONSOLE", "DEBUG");
        return instance;
    }
}
