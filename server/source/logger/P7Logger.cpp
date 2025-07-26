#include "P7Logger.h"

#include <filesystem>


namespace server {
    P7Logger::P7Logger(std::string const &name)  {
        client_ = P7_Create_Client(TM("/P7.Sink=Console; /P7.Dir=./logs/; /P7.Format=\"[%lv] %cn %tf %ms\""));
        trace_ = P7_Create_Trace(client_, TM(name.c_str()));
        trace_->Register_Module(TM(name.c_str()), &module_);
    }

    P7Logger::~P7Logger() {
        if (trace_) trace_->Release();
        if (client_) client_->Release();
    }

    void P7Logger::log(LogLevel level, std::string const &message) {
        std::lock_guard lock(mutex_);
        switch (level) {
            case LogLevel::Trace:
                trace_->P7_TRACE(module_, TM("%s"), message.c_str());
            break;
            case LogLevel::Debug:
                trace_->P7_DEBUG(module_, TM("%s"), message.c_str());
            break;
            case LogLevel::Info:
                trace_->P7_INFO(module_, TM("%s"), message.c_str());
            break;
            case LogLevel::Warning:
                trace_->P7_WARNING(module_, TM("%s"), message.c_str());
            break;
            case LogLevel::Error:
                trace_->P7_ERROR(module_, TM("%s"), message.c_str());
            break;
            case LogLevel::Critical:
                trace_->P7_CRITICAL(module_, TM("%s"), message.c_str());
            break;
        }
    }

    void P7Logger::log(LogLevel level, std::string const &message, LogContext const& ctx) {
        char const* file = std::filesystem::path(ctx.file).filename().c_str();

        switch (level) {
            case LogLevel::Trace:
                trace_->P7_TRACE(module_, TM("%s:%d (func %s):\t%s"), file, ctx.line, ctx.function, message.c_str());
            break;
            case LogLevel::Debug:
                trace_->P7_DEBUG(module_, TM("%s:%d (func %s):\t%s"), file, ctx.line, ctx.function, message.c_str());
            break;
            case LogLevel::Info:
                trace_->P7_INFO(module_, TM("%s:%d (func %s):\t%s"), file, ctx.line, ctx.function, message.c_str());
            break;
            case LogLevel::Warning:
                trace_->P7_WARNING(module_, TM("%s:%d (func %s):\t%s"), file, ctx.line, ctx.function, message.c_str());
            break;
            case LogLevel::Error:
                trace_->P7_ERROR(module_, TM("%s:%d (func %s):\t%s"), file, ctx.line, ctx.function, message.c_str());
            break;
            case LogLevel::Critical:
                trace_->P7_CRITICAL(module_, TM("%s:%d (func %s):\t%s"), file, ctx.line, ctx.function, message.c_str());
            break;
        }

    }

    void P7Logger::trace(std::string const &message) {
        log(LogLevel::Trace, message);
    }

    void P7Logger::trace(std::string const &message, LogContext const& ctx) {
        log(LogLevel::Trace, message, ctx);
    }

    void P7Logger::debug(std::string const &message) {
        log(LogLevel::Debug, message);
    }

    void P7Logger::debug(std::string const &message, LogContext const& ctx) {
        log(LogLevel::Debug, message, ctx);
    }

    void P7Logger::info(std::string const &message) {
        log(LogLevel::Info, message);
    }

    void P7Logger::info(std::string const &message, LogContext const& ctx) {
        log(LogLevel::Info, message, ctx);
    }

    void P7Logger::warn(std::string const &message) {
        log(LogLevel::Warning, message);
    }

    void P7Logger::warn(std::string const &message, LogContext const& ctx) {
        log(LogLevel::Warning, message, ctx);
    }

    void P7Logger::error(std::string const &message) {
        log(LogLevel::Error, message);
    }

    void P7Logger::error(std::string const &message, LogContext const& ctx) {
        log(LogLevel::Error, message, ctx);
    }

    void P7Logger::critical(std::string const &message) {
        log(LogLevel::Critical, message);
    }

    void P7Logger::critical(std::string const &message, LogContext const& ctx) {
        log(LogLevel::Critical, message, ctx);
    }
}
