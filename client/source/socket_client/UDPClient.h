#ifndef UDPSERVER_H
#define UDPSERVER_H
#include <functional>
#include <string>
#include <cstdint>
#include <memory>
#include <netinet/in.h>

#include "ISocketClient.h"
#include "logger/ILogger.h"

namespace client {
    class UDPClient : public ISocketClient {
        std::string const server_ip_address_;
        std::uint16_t server_port_;
        sockaddr_in server_binary_addr_;
        int sock_fd_;
        UDPCallback callback_;

        std::shared_ptr<ILogger> logger_;

        static int set_nonblocking(int fd);
    public:
        UDPClient(std::string  server_ip_address, std::uint16_t server_port, std::shared_ptr<ILogger> logger);

        UDPClient(UDPClient const &) = delete;

        ~UDPClient() override;

        bool setup();

        void send(UDPRequest const& request, std::uint16_t timeout_ms, UDPCallback const& callback) override;
    };
}


#endif //UDPSERVER_H
