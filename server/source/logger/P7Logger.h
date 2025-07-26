#ifndef P7LOGGER_H
#define P7LOGGER_H
#include <mutex>

#include <P7_Client.h>
#include <P7_Trace.h>

#include "ILogger.h"

namespace server {
    class P7Logger : public ILogger {
        std::mutex mutex_;

        IP7_Client *client_ = nullptr;
        IP7_Trace *trace_ = nullptr;
        IP7_Trace::hModule module_ = nullptr;

    public:
        explicit P7Logger(std::string const &name = "AppLogger");

        ~P7Logger() override;

        void log(LogLevel level, std::string const &message) override;

        void log(LogLevel level, std::string const &message, LogContext const &ctx) override;

        void trace(std::string const &message) override;

        void trace(std::string const &message, LogContext const &ctx) override;

        void debug(std::string const &message) override;

        void debug(std::string const &message, LogContext const &ctx) override;

        void info(std::string const &message) override;

        void info(std::string const &message, LogContext const &ctx) override;

        void warn(std::string const &message) override;

        void warn(std::string const &message, LogContext const &ctx) override;

        void error(std::string const &message) override;

        void error(std::string const &message, LogContext const &ctx) override;

        void critical(std::string const &message) override;

        void critical(std::string const &message, LogContext const &ctx) override;
    };
} // namespace server


#endif // P7LOGGER_H
