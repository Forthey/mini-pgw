#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <filesystem>
#include <optional>
#include <fstream>

namespace server {
    using FsPath = std::filesystem::path;

    class FileException : public std::runtime_error {
        std::string full_msg;
        std::string msg;
        std::optional<std::filesystem::path> file_path;

    public:
        explicit FileException(std::string const &msg, std::optional<FsPath> const &file_path)
            : std::runtime_error(""), msg(msg), file_path(file_path) {
            std::ostringstream oss;
            oss << msg;
            if (file_path.has_value()) {
                oss << " (file: " << file_path.value() << ")";
            }
            full_msg = oss.str();
        }

        char const *what() const noexcept override {
            return full_msg.c_str();
        }

        std::optional<FsPath> const &get_file_path() const noexcept {
            return file_path;
        }
    };

    class FileNotFoundException : public FileException {
    public:
        explicit FileNotFoundException(std::optional<FsPath> const &file_path)
            : FileException("File not found", file_path) {
        }
    };

    class FileOpenException : public FileException {
    public:
        explicit FileOpenException(std::optional<FsPath> const &file_path)
            : FileException("Cannot open file", file_path) {
        }
    };

    class FileReadException : public FileException {
    public:
        explicit FileReadException(std::optional<FsPath> const &file_path)
            : FileException("Cannot read from file", file_path) {
        }
    };

    class FileWriteException : public FileException {
    public:
        explicit FileWriteException(std::optional<FsPath> const &file_path)
            : FileException("Cannot write into file", file_path) {
        }
    };

    class FileHandler {
        std::fstream file;
        FsPath path;

    public:
        enum class FileMode {
            Write,
            Read,
            Append
        };

        explicit FileHandler(FsPath const &path, FileMode mode);

        FileHandler(FileHandler const &) = delete;

        FileHandler &operator=(FileHandler const &) = delete;

        std::optional<std::string> readLine();

        void writeLine(std::string_view line);

        std::uint64_t size() const;

        FsPath const &getPath() const;

        ~FileHandler();
    };
}


#endif // FILEHANDLER_H
