#include "SessionManager.h"

namespace server {
    void SessionManager::cleanupLoop() {
        while (true) {
            std::unique_lock lock(mutex_);

            cv_.wait_for(lock, std::chrono::seconds(2ull));

            if (shutting_down_) {
                break;
            }

            auto const now = Clock::now();
            std::vector<std::string> expired;

            for (auto &[imsi, data]: active_sessions_) {
                if (data.expiration_timestamp <= now) {
                    expired.push_back(imsi);
                }
            }

            for (auto &imsi: expired) {
                active_sessions_.erase(imsi);
                logger_->debug(
                    std::format("Session expired: {}", imsi), WITH_CONTEXT
                );
            }
        }
    }

    SessionManager::SessionManager(int const ttl_seconds, int const graceful_shutdown_rate,
                                   std::shared_ptr<ILogger> logger)
        : ttl_(std::chrono::seconds(ttl_seconds)), shutdown_rate_(graceful_shutdown_rate), shutting_down_(false),
          logger_(std::move(logger)) {
        cleaner_thread_ = std::thread(&SessionManager::cleanupLoop, this);
    }

    SessionManager::~SessionManager() {
        if (not shutting_down_) {
            SessionManager::shutdown(false);
        }
        if (cleaner_thread_.joinable()) {
            cleaner_thread_.join();
        }
    }

    void SessionManager::setBlacklist(std::unordered_set<std::string> blacklist) {
        blacklist_ = std::move(blacklist);
    }

    bool SessionManager::upsertSession(std::string const &imsi) {
        std::lock_guard lock(mutex_);

        if (shutting_down_) {
            logger_->warn(
                std::format("Cannot create session {} during graceful shutdown", imsi), WITH_CONTEXT
            );
            return false;
        }

        if (blacklist_.contains(imsi)) {
            logger_->warn(
                std::format("IMSI {} in blacklist, create session rejected", imsi), WITH_CONTEXT
            );
            return false;
        }

        active_sessions_[imsi] = SessionData{
            .expiration_timestamp = Clock::now() + ttl_
        };
        return true;
    }

    std::optional<SessionData> SessionManager::getSession(std::string const &imsi) const {
        if (not active_sessions_.contains(imsi)) {
            return std::nullopt;
        }
        return active_sessions_.at(imsi);
    }

    bool SessionManager::isBlacklisted(std::string const &imsi) const {
        return blacklist_.contains(imsi);
    }

    void SessionManager::shutdown(bool const graceful) {
        if (not graceful) {
            logger_->warn("Performing non-graceful shutdown", WITH_CONTEXT);
        } {
            std::lock_guard lock(mutex_);
            if (shutting_down_) {
                logger_->warn("Sessions shutdown has been called twice", WITH_CONTEXT);
                return;
            }
            shutting_down_ = true;

            cv_.notify_all();
        }

        while (true) {
            std::vector<std::string> sessions_to_remove; {
                std::lock_guard lock(mutex_);

                if (active_sessions_.empty()) {
                    break;
                }

                auto iter = active_sessions_.begin();
                for (int i = 0; i < shutdown_rate_ && iter != active_sessions_.end(); ++i) {
                    sessions_to_remove.push_back(iter->first);
                    ++iter;
                }

                for (auto &imsi: sessions_to_remove) {
                    active_sessions_.erase(imsi);
                    logger_->debug(
                        std::format("Offloaded session: {}", imsi), WITH_CONTEXT
                    );
                }
            }

            if (graceful) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }

        logger_->info("All sessions offloaded, shutdown complete", WITH_CONTEXT);
    }
} // namespace server
