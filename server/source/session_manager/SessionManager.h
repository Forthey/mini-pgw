#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H
#include <chrono>
#include <unordered_map>
#include <unordered_set>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "ISessionManager.h"
#include "logger/ILogger.h"

namespace server {
    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    struct SessionData {
        TimePoint expiration_timestamp;
    };

    class SessionManager : public ISessionManager<std::string, SessionData> {
        std::mutex mutex_;
        std::condition_variable cv_;

        std::unordered_map<std::string, SessionData> active_sessions_;
        std::unordered_set<std::string> blacklist_;

        std::chrono::seconds ttl_;
        int shutdown_rate_;
        bool shutting_down_;
        std::thread cleaner_thread_;

        std::shared_ptr<ILogger> logger_;


        void cleanupLoop();

    public:
        explicit SessionManager(int ttl_seconds, int graceful_shutdown_rate, std::shared_ptr<ILogger> logger);

        ~SessionManager() override;

        void setBlacklist(std::unordered_set<std::string> blacklist);

        bool upsertSession(std::string const &imsi) override;

        std::optional<SessionData> getSession(std::string const &imsi) const override;

        bool isBlacklisted(std::string const &imsi) const;

        void shutdown(bool graceful);
    };
} // namespace server


#endif //SESSIONMANAGER_H
