#ifndef LOGGERFACTORY_H
#define LOGGERFACTORY_H
#include <memory>

#include "ILogger.h"

namespace client {
    class LoggerFactory {
    public:
        static std::shared_ptr<ILogger> createLogger(std::string const &name, std::string const &log_dir, std::string const& log_level);

        static std::shared_ptr<ILogger> getSingletonLogger();
    };
} // namespace server

#endif //LOGGERFACTORY_H
