#include <iostream>
#include <memory>

#include "Server.h"
#include "logger/LoggerFactory.h"


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <path to config>" << std::endl;
        return EXIT_FAILURE;
    }
    std::string const config_path = argv[1];

    std::shared_ptr<server::ILogger> main_logger = server::LoggerFactory::getSingletonLogger();
    try {
        std::shared_ptr<server::IServer> server = std::make_shared<server::Server>(config_path);

        if (not server->setup()) {
            return EXIT_FAILURE;
        }

        server->listen();
    } catch (std::exception const& e) {
        main_logger->critical(e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
