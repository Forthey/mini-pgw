#include <memory>
#include <thread>

#include "http_server/HTTPlibServer.h"
#include "http_server/IHTTPServer.h"
#include "logger/LoggerFactory.h"
#include "session_manager/SessionManager.h"
#include "socket_server/UDPServer.h"


int main() {
    std::shared_ptr<server::ILogger> logger = server::LoggerFactory::createLogger("Main");

    std::shared_ptr<server::ISessionManager> session_manager = std::make_shared<server::SessionManager>(
        10, 10, logger
    );

    std::shared_ptr<server::ISocketServer> udp_server = std::make_shared<server::UDPServer>(
        static_cast<std::uint16_t>(8080),
        logger,
        [&session_manager](server::UDPRequest const &request) -> server::UDPResponse {
            if (not session_manager->upsertSession(request.data)) {
                return {
                    .data = "rejected"
                };
            }
            return {
                .data = "created"
            };
        }
    );

    std::shared_ptr<server::IHTTPServer> http_server = std::make_shared<server::HTTPlibServer>(
        "0.0.0.0", 8080,
        session_manager, [&] {
            udp_server->shutdown();
            http_server->shutdown();
            session_manager->shutdown(true);
        }, logger
    );

    if (not udp_server->start()) {
        logger->critical("Failed to start UDPServer");
        return EXIT_FAILURE;
    }

    std::thread
            udp_server_thread(&server::ISocketServer::startPolling, udp_server.get()),
            http_server_thread(&server::IHTTPServer::listen, http_server.get());

    udp_server_thread.join();
    http_server_thread.join();

    return EXIT_SUCCESS;
}
