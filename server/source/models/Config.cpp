#include "Config.h"

#include <nlohmann/json.hpp>


namespace server {
    std::optional<ParseError> ConfigParser::deserialize(std::string const &json, Config &model) {
        try {
            nlohmann::json j = nlohmann::json::parse(json);

            model = {
                .udp_ip = j.at("udp_ip").get<std::string>(),
                .udp_port = j.at("udp_port").get<std::uint16_t>(),
                .session_timeout_sec = j.at("session_timeout_sec").get<std::uint16_t>(),
                .cdr_file = j.at("cdr_file").get<std::string>(),
                .http_port = j.at("http_port").get<std::uint16_t>(),
                .graceful_shutdown_rate = j.at("graceful_shutdown_rate").get<int>(),
                .log_dir = j.at("log_dir").get<std::string>(),
                .log_level = j.at("log_level").get<std::string>(),
                .blacklist = j.at("blacklist").get<std::vector<std::string>>()
            };

        } catch (nlohmann::json::parse_error const& e) {
            return e.what();
        } catch (nlohmann::json::type_error const& e) {
            return e.what();
        } catch (nlohmann::json::out_of_range& e) {
            return e.what();
        }

        return std::nullopt;
    }
} // namespace server
