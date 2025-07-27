#ifndef ISESSIONMANAGER_H
#define ISESSIONMANAGER_H
#include <optional>

namespace server {
    template <typename SessionKey, typename SessionData>
    class ISessionManager {
    public:
        virtual ~ISessionManager() = default;

        virtual bool upsertSession(SessionKey const&) = 0;

        virtual std::optional<SessionData> getSession(SessionKey const&) const = 0;
    };
} // namespace server

#endif // ISESSIONMANAGER_H
