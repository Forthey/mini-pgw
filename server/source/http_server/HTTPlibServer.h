#ifndef HTTPLIBSERVER_H
#define HTTPLIBSERVER_H
#include <string>
#include <cstdint>

#include <httplib.h>

#include "IHTTPServer.h"
#include "logger/ILogger.h"
#include "session_manager/ISessionManager.h"

namespace server {
    using ShutdownFunc = std::function<void()>;

    class HTTPlibServer : public IHTTPServer {
        httplib::Server server_;
        std::string const host_;
        std::uint16_t const port_;

        std::shared_ptr<ISessionManager> session_manager_;
        std::shared_ptr<ILogger> logger_;

        ShutdownFunc shutdown_callback_;

        static inline void buildHTTPException(httplib::Response& res, std::uint16_t status_code, std::string const& detail);

        void checkSubscriber(httplib::Request const& req, httplib::Response& res) const;

        void stop(httplib::Request const& req, httplib::Response& res);
    public:
        HTTPlibServer(std::string host, std::uint16_t port,
            std::shared_ptr<ISessionManager> session_manager, ShutdownFunc shutdown_callback, std::shared_ptr<ILogger> logger);

        ~HTTPlibServer() override;

        void listen() override;

        void shutdown() override;
    };
} // namespace server


#endif // HTTPLIBSERVER_H
