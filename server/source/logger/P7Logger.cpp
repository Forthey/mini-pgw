#include "P7Logger.h"

#include <filesystem>
#include <format>
#include <unordered_map>


namespace server {
    P7Logger::P7Logger(std::string const &name, std::string const &log_dir,
                       std::string const& log_level) {
        std::pmr::unordered_map<std::string, std::uint8_t> const log_level_str_to_num = {
            {"DEBUG", 1},
            {"INFO", 2},
            {"WARNING", 3},
            {"ERROR", 4},
            {"CRITICAL", 5}
        };

        if (not log_level_str_to_num.contains(log_level)) {
            throw std::invalid_argument(std::format("Invalid log level {}", log_level));
        }

        std::string const sink_option = log_dir == "CONSOLE" ? "Console" : "FileTxt";

        client_ = P7_Create_Client(TM(
            std::format(
                "/P7.Sink={} /P7.Dir=\"{}\" /P7.Roll=10mb /P7.Trc.Verb={} /P7.Format=\"[%lv] %cn %tf %ms\"",
                sink_option, log_dir, log_level_str_to_num.at(log_level)
            ).c_str()
        ));
        trace_ = P7_Create_Trace(client_, TM(name.c_str()));
        trace_->Register_Module(TM(name.c_str()), &module_);
    }

    P7Logger::~P7Logger() {
        if (trace_ != nullptr) {
            trace_->Release();
        }
        if (client_ != nullptr) {
            client_->Release();
        }
    }

    void P7Logger::log(LogLevel level, std::string const &message) {
        std::lock_guard lock(mutex_);
        switch (level) {
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

    void P7Logger::log(LogLevel level, std::string const &message, LogContext const &ctx) {
        char const *file = std::filesystem::path(ctx.file).filename().c_str();

        switch (level) {
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

    void P7Logger::debug(std::string const &message) {
        log(LogLevel::Debug, message);
    }

    void P7Logger::debug(std::string const &message, LogContext const &ctx) {
        log(LogLevel::Debug, message, ctx);
    }

    void P7Logger::info(std::string const &message) {
        log(LogLevel::Info, message);
    }

    void P7Logger::info(std::string const &message, LogContext const &ctx) {
        log(LogLevel::Info, message, ctx);
    }

    void P7Logger::warn(std::string const &message) {
        log(LogLevel::Warning, message);
    }

    void P7Logger::warn(std::string const &message, LogContext const &ctx) {
        log(LogLevel::Warning, message, ctx);
    }

    void P7Logger::error(std::string const &message) {
        log(LogLevel::Error, message);
    }

    void P7Logger::error(std::string const &message, LogContext const &ctx) {
        log(LogLevel::Error, message, ctx);
    }

    void P7Logger::critical(std::string const &message) {
        log(LogLevel::Critical, message);
    }

    void P7Logger::critical(std::string const &message, LogContext const &ctx) {
        log(LogLevel::Critical, message, ctx);
    }
}
