#ifndef ISERVER_H
#define ISERVER_H

namespace server {
    class IServer {
    public:
        virtual ~IServer() = default;

        virtual bool setup() = 0;

        virtual void listen() = 0;

        virtual void shutdown() = 0;
    };
}

#endif // ISERVER_H
