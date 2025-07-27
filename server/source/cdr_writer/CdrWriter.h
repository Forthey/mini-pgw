#ifndef CDRWRITER_H
#define CDRWRITER_H
#include "FileHandler.h"

namespace server {
    class CdrWriter {
        FileHandler file;

        static std::string getTimestamp();
    public:
        explicit CdrWriter(std::filesystem::path const &path);

        void write_data(std::string const& imsi, std::string const& action);
    };
}


#endif //CDRWRITER_H
