#ifndef IJSONMODELPARSER_H
#define IJSONMODELPARSER_H
#include <optional>
#include <string>

namespace client {
    using ParseError = std::string;

    template <typename Model>
    struct IJsonModelDeserializer {
        virtual ~IJsonModelDeserializer() = default;

        virtual std::optional<ParseError> deserialize(std::string const& json, Model &) = 0;
    };
} // namespace server

#endif // IJSONMODELPARSER_H
