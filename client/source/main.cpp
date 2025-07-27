#include <cstdlib>
#include <format>
#include <iostream>

#include "logger/LoggerFactory.h"
#include "socket_client/UDPClient.h"


int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <config path> <imsi>" << std::endl;
    }

    std::string const config_file_path = argv[1];
    std::string const IMSI = argv[2];

    auto logger = client::LoggerFactory::createLogger("Main", "CONSOLE", "DEBUG");

    try {
        auto client = std::make_unique<client::UDPClient>(config_file_path, logger);

        if (not client->setup()) {
            logger->critical("Failed to setup client");
            return EXIT_FAILURE;
        }


        client->send({.data = IMSI}, [](client::UDPResponse const &response) {
            response.ctx.logger->info(
                std::format("Received from server: {}", response.data), WITH_CONTEXT
            );
        }, 0);
    } catch (std::exception const &e) {
        logger->critical(e.what(), WITH_CONTEXT);
    }
    return EXIT_SUCCESS;
}
