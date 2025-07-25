#include "LoggerFactory.h"

#include "P7Logger.h"

namespace server {
    std::shared_ptr<ILogger> LoggerFactory::createLogger(std::string const &moduleName) {
        return std::make_shared<P7Logger>(moduleName);
    }

    std::shared_ptr<ILogger> LoggerFactory::getSingletonLogger(std::string const &moduleName = "MainModule") {
        static std::shared_ptr<ILogger> instance = std::make_shared<P7Logger>(moduleName);
        return instance;
    }
}
