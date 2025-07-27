#ifndef HTTPEXCEPTIONRESPONSE_H
#define HTTPEXCEPTIONRESPONSE_H
#include "models/IJsonModel.h"

namespace server {
    struct HTTPExceptionResponse : IJsonModel {
        std::string detail_;

        ~HTTPExceptionResponse() override = default;

        std::string serialize() const override;

        std::optional<ParseError> deserialize(const std::string &json) override;
    };
} // namespace server

#endif //HTTPEXCEPTIONRESPONSE_H
