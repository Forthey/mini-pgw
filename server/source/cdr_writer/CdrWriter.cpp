//
// Created by forthey on 7/27/25.
//

#include "CdrWriter.h"

#include <chrono>
#include <sstream>

namespace server {
    std::string CdrWriter::getTimestamp() {
        const auto now = std::chrono::system_clock::now();
        const auto time = std::chrono::system_clock::to_time_t(now);
        const auto ms = duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

        std::ostringstream oss;
        oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S")
            << '.' << std::setw(3) << std::setfill('0') << ms.count();
        return oss.str();
    }

    CdrWriter::CdrWriter(std::filesystem::path const &path) : file(path, FileHandler::FileMode::Append) {
    }

    void CdrWriter::write_data(std::string const &imsi, std::string const &action) {
        file.writeLine(std::format("{} {} {}", getTimestamp(), imsi, action));
    }
}
