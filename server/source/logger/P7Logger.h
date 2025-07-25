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
        explicit P7Logger(std::string const &name = "AppLogger") {
            client_ = P7_Create_Client(TM("/P7.Sink=FileTxt; /P7.Dir=./logs/"));
            trace_ = P7_Create_Trace(client_, TM(name.c_str()));
            trace_->Register_Module(TM(name.c_str()), &module_);
        }

        ~P7Logger() override {
            if (trace_) trace_->Release();
            if (client_) client_->Release();
        }

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
