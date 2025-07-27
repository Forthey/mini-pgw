#ifndef IJSONMODELPARSER_H
#define IJSONMODELPARSER_H
#include <string>

namespace server {
    using ParseError = std::string;

    template <typename Model>
    struct IJsonModelSerializer {
        virtual ~IJsonModelSerializer() = default;

        virtual std::string serialize(Model const&) const = 0;
    };
} // namespace server

#endif // IJSONMODELPARSER_H
