#include "UDPClient.h"

#include <cstring>
#include <format>
#include <utility>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>


namespace client {
    UDPClient::UDPClient(std::string server_ip_address, std::uint16_t server_port, std::shared_ptr<ILogger> logger)
        : server_ip_address_(std::move(server_ip_address)), server_port_(server_port), sock_fd_(-1),
          logger_(std::move(logger)) {
        logger_->debug("Creating UDPClient instance", WITH_CONTEXT);

        server_binary_addr_ = {
            .sin_family = AF_INET,
            .sin_port = htons(server_port_),
        };

        if (inet_pton(AF_INET, server_ip_address_.c_str(), &server_binary_addr_.sin_addr) <= 0) {
            logger_->critical(
                std::format("Failed to parse ip address {}", server_ip_address_),WITH_CONTEXT
            );
            throw std::invalid_argument("Failed to parse ip address");
        }
    }

    UDPClient::~UDPClient() {
        logger_->debug("Deleting UDPClient instance", WITH_CONTEXT);

        if (sock_fd_ != -1) {
            close(sock_fd_);
        }
    }

    int UDPClient::set_nonblocking(int const fd) {
        int flags = fcntl(fd, F_GETFL, 0);
        if (flags == -1) {
            return -1;
        }
        return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    }

    bool UDPClient::setup() {
        logger_->info("Setting up UDPClient...", WITH_CONTEXT);

        sock_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock_fd_ < 0) {
            logger_->critical(
                std::format("Failed to create socket ({}): {}", errno, std::strerror(errno)), WITH_CONTEXT
            );
            return false;
        }

        if (set_nonblocking(sock_fd_) < 0) {
            logger_->critical(
                std::format("Failed to set nonblocking mode ({}): {}", errno, std::strerror(errno)),WITH_CONTEXT
            );
        }

        logger_->info(
            std::format("UDP client configured, ready to send to {}:{}", server_ip_address_, server_port_),WITH_CONTEXT
        );
        return true;
    }

    void UDPClient::send(UDPRequest const &request, std::uint16_t const timeout_ms, UDPCallback const &callback) {
        ssize_t bytes_sent = sendto(
            sock_fd_,
            request.data.c_str(),
            request.data.size(),
            0,
            reinterpret_cast<sockaddr *>(&server_binary_addr_),
            sizeof(server_binary_addr_)
        );

        if (bytes_sent < 0) {
            logger_->error(
                std::format("Ошибка отправки ({}): {}", errno, std::strerror(errno)), WITH_CONTEXT
            );
            return;
        }

        logger_->debug(
            std::format("Sent {} bytes to the server {}:{}", bytes_sent, server_ip_address_, server_port_),
            WITH_CONTEXT
        );

        int epoll_fd = epoll_create1(0);
        if (epoll_fd < 0) {
            logger_->error(
                std::format("Failed to create epoll ({}): {}", errno, std::strerror(errno)), WITH_CONTEXT
            );
            return;
        }

        epoll_event event{};
        event.events = EPOLLIN;
        event.data.fd = sock_fd_;

        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock_fd_, &event)) {
            logger_->error(
                std::format("Failed to bind socket to epoll ({}): {}", errno, std::strerror(errno)), WITH_CONTEXT
            );
            close(epoll_fd);
            return;
        }

        constexpr int MAX_EVENTS = 10;
        constexpr int BUFFER_SIZE = 65536;
        epoll_event events[MAX_EVENTS];
        char buffer[BUFFER_SIZE];
        bool response_received = false;

        logger_->debug(
            std::format("Waiting for response (timeout {}ms) ...", timeout_ms), WITH_CONTEXT
        );

        while (true) {
            int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, timeout_ms);

            if (num_events < 0) {
                if (errno == EINTR) {
                    logger_->warn("Epoll was interrupted by system call", WITH_CONTEXT);
                    continue;
                }

                logger_->error(
                    std::format("Failed to poll ({}): {}", errno, std::strerror(errno)), WITH_CONTEXT
                );
                break;
            }

            if (num_events == 0) {
                logger_->warn("Did not receive response from server, timout reached", WITH_CONTEXT);
                break;
            }

            for (int i = 0; i < num_events; ++i) {
                if (events[i].data.fd == sock_fd_) {
                    sockaddr_in from_binary_addr{};
                    socklen_t from_len = sizeof(from_binary_addr);

                    ssize_t bytes_received = recvfrom(
                        sock_fd_,
                        buffer,
                        BUFFER_SIZE - 1,
                        0,
                        reinterpret_cast<sockaddr *>(&from_binary_addr),
                        &from_len
                    );

                    if (bytes_received < 0) {
                        if (errno != EAGAIN && errno != EWOULDBLOCK) {
                            logger_->error(
                                std::format("Failed to read data ({}): {}", errno, std::strerror(errno)),
                                WITH_CONTEXT
                            );
                        }
                        continue;
                    }

                    char from_ip[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &from_binary_addr.sin_addr, from_ip, INET_ADDRSTRLEN);
                    uint16_t from_port = ntohs(from_binary_addr.sin_port);

                    if (from_binary_addr.sin_port == server_binary_addr_.sin_port &&
                        from_binary_addr.sin_addr.s_addr == server_binary_addr_.sin_addr.s_addr) {
                        buffer[bytes_received] = '\0';

                        callback({
                            .data = buffer
                        });

                        response_received = true;
                        break;
                    } else {
                        logger_->warn(
                            std::format("Received udp packet from unknown address {}:{}", from_ip, from_port),
                            WITH_CONTEXT
                        );
                    }
                }
            }

            if (response_received) {
                break;
            }
        }

        close(epoll_fd);
    }
}
