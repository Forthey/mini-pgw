#include <memory>

#include "logger/LoggerFactory.h"
#include "socket_server/UDPServer.h"


int main() {
    std::shared_ptr<server::ILogger> logger = server::LoggerFactory::createLogger("Main");

    auto udp_server = std::make_shared<server::UDPServer>(
        static_cast<std::uint16_t>(8080),
        server::LoggerFactory::createLogger("UDPServer"),
        [](server::UDPRequest const &request) -> server::UDPResponse {
            return {
                .data = request.data
            };
        }
    );

    if (not udp_server->start()) {
        logger->critical("Failed to start UDPServer");
        return EXIT_FAILURE;
    }

    udp_server->startPolling();
}
