#include "Server.h"

#include <format>
#include <fstream>
#include <sstream>

#include "http_server/HTTPlibServer.h"
#include "logger/LoggerFactory.h"
#include "session_manager/SessionManager.h"
#include "socket_server/UDPServer.h"

namespace server {
    Server::Server(std::string const &config_file_path) {
        logger_ = LoggerFactory::getSingletonLogger();

        std::ifstream config_file(config_file_path);

        if (not config_file.is_open()) {
            logger_->critical(
                std::format("Could not open config file {}", config_file_path), WITH_CONTEXT
            );
            throw std::invalid_argument("Could not open config file");
        }

        std::ostringstream buffer;
        buffer << config_file.rdbuf();
        std::string config_file_content = buffer.str();

        buffer.clear();
        config_file.close();

        if (auto const err = ConfigParser().deserialize(config_file_content, serverConfig_); err.has_value()) {
            logger_->critical(
                std::format("Failed to parse config file: {}", err.value()), WITH_CONTEXT
            );
            throw std::invalid_argument("Failed to parse config file");
        }

        logger_ = LoggerFactory::createLogger("Server", serverConfig_.log_dir, serverConfig_.log_level);
    }

    Server::~Server() {
        Server::shutdown();
    }

    bool Server::setup() {
        session_manager_ = std::make_shared<SessionManager>(
            serverConfig_.session_timeout_sec, serverConfig_.graceful_shutdown_rate, serverConfig_.cdr_file, logger_
        );
        std::dynamic_pointer_cast<SessionManager>(session_manager_)->setBlacklist(serverConfig_.blacklist);

        sub_servers_.emplace_back(
            std::make_shared<UDPServer>(
                serverConfig_.udp_port,
                logger_,
                [this](UDPRequest const &request) -> UDPResponse {
                    if (not session_manager_->upsertSession(request.data)) {
                        return {
                            .data = "rejected"
                        };
                    }
                    return {
                        .data = "created"
                    };
                }
            )
        );

        sub_servers_.emplace_back(
            std::make_shared<HTTPlibServer>(
                "0.0.0.0", serverConfig_.http_port,
                session_manager_, [this] {
                    shutdown();
                }, logger_
            )
        );

        for (auto &server: sub_servers_) {
            if (not server->setup()) {
                logger_->critical("Failed to start one of the servers", WITH_CONTEXT);
                return false;
            }
        }

        return true;
    }

    void Server::listen() {
        std::vector<std::thread> server_threads;

        for (auto &server: sub_servers_) {
            server_threads.emplace_back(&IServer::listen, server.get());
        }
        for (auto &server: server_threads) {
            server.join();
        }
    }

    void Server::shutdown() {
        for (auto &server: sub_servers_) {
            server->shutdown();
        }
        session_manager_->shutdown(true);
    }
}
