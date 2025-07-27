#ifndef ISOCKETCLIENT_H
#define ISOCKETCLIENT_H
#include <string>

#include "logger/ILogger.h"

namespace client {
    struct DataContext {
        std::shared_ptr<ILogger> logger;
    };

    struct UDPRequest {
        std::string const data;
    };

    struct UDPResponse {
        std::string const data;
        DataContext ctx;
    };

    using UDPCallback = std::function<void(UDPResponse const &)>;

    class ISocketClient {
    public:
        virtual ~ISocketClient() = default;

        virtual void send(UDPRequest const&, UDPCallback const&, std::uint16_t) = 0;
    };
}


#endif // ISOCKETCLIENT_H
