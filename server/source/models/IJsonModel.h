#ifndef IJSONMODEL_H
#define IJSONMODEL_H
#include <optional>
#include <string>

namespace server {
    using ParseError = std::string;

    struct IJsonModel {
        virtual ~IJsonModel() = default;

        virtual std::string serialize() const = 0;

        virtual std::optional<ParseError> deserialize(std::string const& json) = 0;
    };
} // namespace server

#endif //IJSONMODEL_H
