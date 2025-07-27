#ifndef MOCKLOGGER_H
#define MOCKLOGGER_H
#include <gmock/gmock.h>
#include "logger/ILogger.h"

namespace server {
    class MockLogger : public ILogger {
    public:
        MOCK_METHOD(void, log, (LogLevel level, const std::string &message), (override));
        MOCK_METHOD(void, log, (LogLevel level, const std::string &message, LogContext const &ctx), (override));
        MOCK_METHOD(void, debug, (const std::string &message), (override));
        MOCK_METHOD(void, debug, (const std::string &message, LogContext const &ctx), (override));
        MOCK_METHOD(void, info, (const std::string &message), (override));
        MOCK_METHOD(void, info, (const std::string &message, LogContext const &ctx), (override));
        MOCK_METHOD(void, warn, (const std::string &message), (override));
        MOCK_METHOD(void, warn, (const std::string &message, LogContext const &ctx), (override));
        MOCK_METHOD(void, error, (const std::string &message), (override));
        MOCK_METHOD(void, error, (const std::string &message, LogContext const &ctx), (override));
        MOCK_METHOD(void, critical, (const std::string &message), (override));
        MOCK_METHOD(void, critical, (const std::string &message, LogContext const &ctx), (override));
        ~MockLogger() override = default;
    };
} // namespace server

#endif //MOCKLOGGER_H
