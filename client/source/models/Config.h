#ifndef CONFIG_H
#define CONFIG_H
#include <vector>
#include <cstdint>

#include "IJsonModelDeserializer.h"

namespace client {
    struct Config {
        std::string server_ip;
        std::uint16_t server_port;
        std::uint16_t response_timeout_sec;
        std::string log_dir;
        std::string log_level;
    };

    class ConfigParser : IJsonModelDeserializer<Config> {
    public:
        ~ConfigParser() override = default;

        std::optional<ParseError> deserialize(std::string const &json, Config &model) override;
    };
} // namespace server

#endif // CONFIG_H
