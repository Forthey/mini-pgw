#ifndef SERVER_H
#define SERVER_H
#include <memory>
#include <string>

#include "IServer.h"
#include "logger/ILogger.h"
#include "models/Config.h"
#include "session_manager/ISessionManager.h"

namespace server {
    class Server : public IServer {
        Config serverConfig_;
        std::vector<std::shared_ptr<IServer> > sub_servers_;
        std::shared_ptr<ISessionManager> session_manager_;

        std::shared_ptr<ILogger> logger_;

    public:
        explicit Server(std::string const &config_file_path);

        ~Server() override;

        bool setup() override;

        void listen() override;

        void shutdown() override;
    };
}


#endif //SERVER_H
