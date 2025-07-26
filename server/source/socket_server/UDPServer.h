#ifndef UDPSERVER_H
#define UDPSERVER_H
#include <functional>
#include <string>
#include <cstdint>
#include <memory>

#include "ISocketServer.h"
#include "logger/ILogger.h"

namespace server {
    struct UDPRequest {
        std::string const ip_address;
        std::uint16_t const port;
        std::string const data;
    };

    struct UDPResponse {
        std::string const data;
    };

    using UDPCallback = std::function<UDPResponse(UDPRequest const &)>;


    class UDPServer : public ISocketServer {
        std::uint16_t server_port_;
        int sock_fd_;
        int epoll_fd_;
        UDPCallback callback_;

        std::shared_ptr<ILogger> logger_;

        static int set_nonblocking(int fd);

    public:
        UDPServer(std::uint16_t port, std::shared_ptr<ILogger> logger, UDPCallback callback);

        UDPServer(UDPServer const &) = delete;

        ~UDPServer() override;

        bool start() override;

        void startPolling() override;
    };
}


#endif //UDPSERVER_H
