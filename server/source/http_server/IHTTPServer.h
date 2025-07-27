#ifndef IHTTPSERVER_H
#define IHTTPSERVER_H

namespace server {
    class IHTTPServer {
    public:
        virtual ~IHTTPServer() = default;

        virtual void listen() = 0;

        virtual void shutdown() = 0;
    };
} // namespace server

#endif //IHTTPSERVER_H
