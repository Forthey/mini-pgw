#ifndef HTTPEXCEPTIONRESPONSE_H
#define HTTPEXCEPTIONRESPONSE_H
#include "models/IJsonModelSerializer.h"

namespace server {
    struct HTTPExceptionResponse {
        std::string detail;
    };

    class HTTPExceptionResponseParser : IJsonModelSerializer<HTTPExceptionResponse> {
    public:
        ~HTTPExceptionResponseParser() override = default;

        std::string serialize(HTTPExceptionResponse const& model) const override;
    };
} // namespace server

#endif // HTTPEXCEPTIONRESPONSE_H
