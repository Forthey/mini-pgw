#include <cstdlib>
#include <format>

#include "logger/LoggerFactory.h"
#include "socket_client/UDPClient.h"


int main() {
    std::string const IMSI = "250991234567890";

    auto logger = client::LoggerFactory::createLogger("Main");

    auto client = std::make_unique<client::UDPClient>(
        "127.0.0.1", 8080,
        logger
    );

    if (not client->setup()) {
        logger->critical("Failed to setup client");
        return EXIT_FAILURE;
    }

    logger->info("Sending IMSI to server...", WITH_CONTEXT);

    client->send({.data = IMSI}, 5000, [logger](client::UDPResponse const &response) {
        logger->info(
            std::format("Received from server: {}", response.data), WITH_CONTEXT
        );
    });
    return EXIT_SUCCESS;
}
