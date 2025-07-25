#ifndef LOGGERFACTORY_H
#define LOGGERFACTORY_H
#include <memory>

#include "ILogger.h"

namespace server {
    class LoggerFactory {
    public:
        static std::shared_ptr<ILogger> createLogger(std::string const &moduleName);

        static std::shared_ptr<ILogger> getSingletonLogger(std::string const &moduleName );
    };
} // namespace server

#endif //LOGGERFACTORY_H
