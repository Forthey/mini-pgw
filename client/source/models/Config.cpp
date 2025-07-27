#include "Config.h"

#include <nlohmann/json.hpp>


namespace client {
    std::optional<ParseError> ConfigParser::deserialize(std::string const &json, Config &model) {
        try {
            nlohmann::json j = nlohmann::json::parse(json);

            model = {
                .server_ip = j.at("server_ip").get<std::string>(),
                .server_port = j.at("server_port").get<std::uint16_t>(),
                .response_timeout_sec = j.at("response_timeout_sec").get<std::uint16_t>(),
                .log_dir = j.at("log_dir").get<std::string>(),
                .log_level = j.at("log_level").get<std::string>(),
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
