#include "UDPServer.h"

#include <cstring>
#include <format>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <netinet/in.h>

namespace server {
    UDPServer::UDPServer(std::uint16_t const port, std::shared_ptr<ILogger> logger, UDPCallback callback)
        : server_port_(port), sock_fd_(-1), event_fd_(-1), epoll_fd_(-1), should_shutdown_(false), callback_(std::move(callback)),
          logger_(std::move(logger)) {
        logger_->debug("Creating UDPServer instance", WITH_CONTEXT);
    }

    UDPServer::~UDPServer() {
        logger_->debug("Deleting UDPServer instance", WITH_CONTEXT);

        if (epoll_fd_ != -1) {
            close(epoll_fd_);
        }

        if (sock_fd_ != -1) {
            close(sock_fd_);
        }
    }

    int UDPServer::set_nonblocking(int const fd) {
        int flags = fcntl(fd, F_GETFL, 0);
        if (flags == -1) {
            return -1;
        }
        return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    }

    bool UDPServer::setup() {
        logger_->info("Starting UDPServer...", WITH_CONTEXT);

        sock_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock_fd_ < 0) {
            logger_->critical(
                std::format("Failed to create socket ({}): {}", errno, std::strerror(errno)), WITH_CONTEXT
            );
            return false;
        }

        int opt = 1;
        if (setsockopt(sock_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            logger_->critical(
                std::format("Failed to set sock opt ({}): {}", errno, std::strerror(errno)),WITH_CONTEXT
            );
            return false;
        }

        if (set_nonblocking(sock_fd_) < 0) {
            logger_->critical(
                std::format("Failed to set nonblocking mode ({}): {}", errno, std::strerror(errno)),WITH_CONTEXT
            );
        }

        sockaddr_in addr{
            .sin_family = AF_INET,
            .sin_port = htons(server_port_),
            .sin_addr = {
                .s_addr = htonl(INADDR_ANY)
            }
        };

        if (bind(sock_fd_, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
            logger_->critical(
                std::format("Bind failed ({}): {}", errno, std::strerror(errno)),WITH_CONTEXT
            );
            return false;
        }

        epoll_fd_ = epoll_create1(0);
        if (epoll_fd_ < 0) {
            logger_->critical(
                std::format("Failed to create epoll ({}): {}", errno, std::strerror(errno)),WITH_CONTEXT
            );
            return false;
        }

        event_fd_ = eventfd(0, EFD_NONBLOCK);
        if (event_fd_ < 0) {
            logger_->critical(
                std::format("Failed to create eventfd ({}): {}", errno, std::strerror(errno)),WITH_CONTEXT
            );
            return false;
        }

        epoll_event ev{};
        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = sock_fd_;
        if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, sock_fd_, &ev) < 0) {
            logger_->critical(
                std::format("Failed to manipulate epoll instance ({}): {}", errno, std::strerror(errno)),WITH_CONTEXT
            );
            return false;
        }

        ev.data.fd = event_fd_;
        if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, event_fd_, &ev) < 0) {
            logger_->critical(
                std::format("Failed to manipulate epoll instance ({}): {}", errno, std::strerror(errno)),WITH_CONTEXT
            );
            return false;
        }

        logger_->info(
            std::format("UDP server started, listening on {}:{}", "localhost", server_port_),WITH_CONTEXT
        );
        return true;
    }

    void UDPServer::listen() {
        constexpr int MAX_EVENTS = 10;
        constexpr int BUFFER_SIZE = 65536;
        char buffer[BUFFER_SIZE];
        epoll_event events[MAX_EVENTS];

        while (not should_shutdown_) {
            logger_->debug("Polling for events", WITH_CONTEXT);

            int const n = epoll_wait(epoll_fd_, events, MAX_EVENTS, -1);
            if (n < 0) {
                if (errno == EINTR) {
                    logger_->warn("Epoll was interrupted by system call", WITH_CONTEXT);
                    continue;
                }
                logger_->critical(
                    std::format("Failed to poll ({}): {}", errno, std::strerror(errno)), WITH_CONTEXT
                );
                break;
            }

            logger_->info(
                std::format("Received {} events", n), WITH_CONTEXT
            );
            for (int i = 0; i < n; ++i) {
                if (events[i].data.fd == event_fd_) {
                    uint64_t dummy;
                    read(event_fd_, &dummy, sizeof(dummy));
                    logger_->info("Stop signal received");
                    return;
                }

                if (events[i].events & EPOLLIN) {
                    sockaddr_in client_addr{};
                    socklen_t client_len = sizeof(client_addr);

                    while (true) {
                        ssize_t const bytes_received = recvfrom(
                            sock_fd_, buffer, BUFFER_SIZE - 1, 0,
                            reinterpret_cast<sockaddr *>(&client_addr), &client_len
                        );
                        if (bytes_received < 0) {
                            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                                break;
                            }
                            logger_->warn(
                                std::format("Failed to read data ({}): {}", errno, std::strerror(errno)), WITH_CONTEXT
                            );
                            break;
                        }

                        std::string const received(buffer, static_cast<std::size_t>(bytes_received));

                        auto const [data] = callback_({
                            .ip_address = "",
                            .port = static_cast<std::uint16_t>(8080),
                            .data = received
                        });

                        if (sendto(sock_fd_, data.c_str(), data.size(), 0,
                                   reinterpret_cast<sockaddr *>(&client_addr), client_len) < 0) {
                            logger_->warn(
                                std::format("Failed to write data ({}): {}", errno, std::strerror(errno)), WITH_CONTEXT
                            );
                        }
                    }
                }
            }
        }
    }

    void UDPServer::shutdown() {
        if (should_shutdown_) {
            return;
        }
        logger_->info("Shutting down UDPServer gracefully...", WITH_CONTEXT);
        std::uint64_t stop_signal = 1;
        write(event_fd_, &stop_signal, sizeof(stop_signal));
    }
}
