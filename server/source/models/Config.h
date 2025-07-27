#ifndef CONFIG_H
#define CONFIG_H
#include <vector>

#include "IJsonModelDeserializer.h"

namespace server {
    struct Config {
        std::string udp_ip;
        int64_t udp_port;
        int64_t session_timeout_sec;
        std::string cdr_file;
        int64_t http_port;
        int64_t graceful_shutdown_rate;
        std::string log_dir;
        std::string log_level;
        std::vector<std::string> blacklist;
    };

    class ConfigParser : IJsonModelDeserializer<Config> {
    public:
        ~ConfigParser() override = default;

        std::optional<ParseError> deserialize(std::string const &json, Config &model) override;
    };
} // namespace server

#endif // CONFIG_H
