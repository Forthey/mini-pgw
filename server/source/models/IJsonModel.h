#ifndef IJSONMODEL_H
#define IJSONMODEL_H
#include <optional>
#include <string>

namespace server {
    using ParseError = std::string;

    class IJsonModel {
    public:
        virtual ~IJsonModel() = default;

        virtual std::string serialize() const = 0;

        virtual std::optional<ParseError> deserialize(const std::string& json) = 0;
    };
} // namespace server

#endif //IJSONMODEL_H
