#ifndef ISOCKETSERVER_H
#define ISOCKETSERVER_H

namespace server {
    class ISocketServer {
    public:
        virtual ~ISocketServer() = default;

        virtual bool start() = 0;

        virtual void startPolling() = 0;
    };
}
#endif //ISOCKETSERVER_H
