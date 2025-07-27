#ifndef UDPSERVER_H
#define UDPSERVER_H
#include <functional>
#include <string>
#include <cstdint>
#include <memory>
#include <netinet/in.h>

#include "ISocketClient.h"
#include "logger/ILogger.h"
#include "models/Config.h"

namespace client {
    class UDPClient : public ISocketClient {
        sockaddr_in server_binary_addr_;
        int sock_fd_;
        UDPCallback callback_;

        std::shared_ptr<ILogger> logger_;

        Config client_config_;

        static int set_nonblocking(int fd);
    public:
        UDPClient(std::string const &config_file_path, std::shared_ptr<ILogger> logger);

        UDPClient(UDPClient const &) = delete;

        ~UDPClient() override;

        bool setup();

        void send(UDPRequest const& request, UDPCallback const& callback, std::uint16_t timeout_ms) override;
    };
}


#endif //UDPSERVER_H
