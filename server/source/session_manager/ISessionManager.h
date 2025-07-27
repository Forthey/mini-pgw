#ifndef ISESSIONMANAGER_H
#define ISESSIONMANAGER_H
#include <chrono>
#include <string>
#include <optional>

namespace server {
    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    struct SessionData {
        TimePoint expiration_timestamp;
    };

    class ISessionManager {
    public:
        virtual ~ISessionManager() = default;

        virtual bool upsertSession(std::string const&) = 0;

        virtual std::optional<SessionData> getSession(std::string const&) const = 0;

        virtual void shutdown(bool graceful) = 0;
    };
} // namespace server

#endif // ISESSIONMANAGER_H
