#ifndef ISOCKETCLIENT_H
#define ISOCKETCLIENT_H
#include <string>

namespace client {
    struct UDPRequest {
        std::string const data;
    };

    struct UDPResponse {
        std::string const data;
    };

    using UDPCallback = std::function<void(UDPResponse const &)>;

    class ISocketClient {
    public:
        virtual ~ISocketClient() = default;

        virtual void send(UDPRequest const&, std::uint16_t, UDPCallback const&) = 0;
    };
}


#endif // ISOCKETCLIENT_H
