#include "FileHandler.h"


namespace server {
    FileHandler::FileHandler(FsPath const &path, FileMode mode) {
        std::ios::openmode result_mode = std::ios::binary;
        switch (mode) {
            case FileMode::Read:
                if (!std::filesystem::exists(path)) {
                    throw FileNotFoundException(path);
                }

            result_mode |= std::ios::in;
            break;
            case FileMode::Write:
                result_mode |= std::ios::out | std::ios::trunc;
            break;
            case FileMode::Append:
                result_mode |= std::ios::app | std::ios::out;
            break;
        }

        file.open(path, result_mode);
        if (!file.is_open()) {
            throw FileOpenException(path);;
        }
    }

    std::optional<std::string> FileHandler::readLine() {
        if (!file.is_open() || !file.good()) {
            throw FileReadException(path);
        }
        std::string line;
        if (!std::getline(file, line)) {
            if (file.eof()) {
                return std::nullopt;
            }
            throw FileReadException(path);
        }
        return line;
    }

    void FileHandler::writeLine(std::string_view line) {
        if (!file.is_open() || !file.good()) {
            throw FileWriteException(path);
        }
        if (!(file << line << "\n")) {
            throw FileWriteException(path);
        }
        file.flush();
    }

    std::uint64_t FileHandler::size() const {
        return std::filesystem::file_size(path);
    }

    FsPath const &FileHandler::getPath() const {
        return path;
    }

    FileHandler::~FileHandler() {
        file.close();
    }
}