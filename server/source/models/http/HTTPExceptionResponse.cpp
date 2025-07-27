#include "HTTPExceptionResponse.h"

#include <nlohmann/json.hpp>

namespace server {
    std::string HTTPExceptionResponse::serialize() const {
        nlohmann::json j;

        j["detail"] = detail_;

        return j.dump();
    }

    std::optional<ParseError> HTTPExceptionResponse::deserialize(const std::string &json) {
        try {
            nlohmann::json j = nlohmann::json::parse(json);

            detail_ = j.at("detail");
        } catch (nlohmann::json::parse_error const& e) {
            return e.what();
        } catch (nlohmann::json::type_error const& e) {
            return e.what();
        } catch (nlohmann::json::out_of_range& e) {
            return e.what();
        }

        return std::nullopt;
    }
}
