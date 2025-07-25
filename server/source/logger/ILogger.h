#ifndef ILOGGER_H
#define ILOGGER_H

#pragma once
#include <string>

namespace server {
#define WITH_CONTEXT {.file=__FILE__, .line=__LINE__, .function=__FUNCTION__}

    struct LogContext {
        char const *file;
        int const line;
        char const *function;
    };

    enum class LogLevel {
        Trace,
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };

    class ILogger {
    public:
        virtual ~ILogger() = default;

        virtual void log(LogLevel level, const std::string &message) = 0;

        virtual void log(LogLevel level, const std::string &message, LogContext const &ctx) = 0;

        virtual void trace(std::string const &message) = 0;

        virtual void trace(std::string const &message, LogContext const &ctx) = 0;

        virtual void debug(std::string const &message) = 0;

        virtual void debug(std::string const &message, LogContext const &ctx) = 0;

        virtual void info(std::string const &message) = 0;

        virtual void info(std::string const &message, LogContext const &ctx) = 0;

        virtual void warn(std::string const &message) = 0;

        virtual void warn(std::string const &message, LogContext const &ctx) = 0;

        virtual void error(std::string const &message) = 0;

        virtual void error(std::string const &message, LogContext const &ctx) = 0;

        virtual void critical(std::string const &message) = 0;

        virtual void critical(std::string const &message, LogContext const &ctx) = 0;
    };
} // namespace server

#endif // ILOGGER_H
