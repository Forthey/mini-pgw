#include "HTTPExceptionResponse.h"

#include <nlohmann/json.hpp>

namespace server {
    std::string HTTPExceptionResponseParser::serialize(HTTPExceptionResponse const &model) const {
        nlohmann::json j;

        j["detail"] = model.detail;

        return j.dump();
    }
}
