#pragma once
#include <filesystem>
#include <fstream>
#include <mutex>
#include <source_location>
#include <utility>

#include "fmt/color.h"

namespace fs = std::filesystem;


/**
 * @brief Log messages, use Logger::Get() for global instance or create instance with a prefix
 */
class Logger
{
public:
    explicit Logger(std::string prefix = "") : title(std::move(prefix)) {}

    // Lazy initialized singleton Logger with empty prefix
    static Logger &Get();
    static void    LogToFile(const bool enable);
    static void    ClearLog();

    void Info(const std::string &message, const std::source_location &location = std::source_location::current()) const;
    void InfoS(const std::string &message) const;

    void Debug(const std::string &message, const std::source_location &location = std::source_location::current()) const;
    void DebugS(const std::string &message) const;

    void Warning(const std::string &message, const std::source_location &location = std::source_location::current()) const;
    void WarningS(const std::string &message) const;

    void Error(const std::string &message, const std::source_location &location = std::source_location::current()) const;
    void ErrorS(const std::string &message) const;

private:
    void log(const std::string &message, const char *category, fmt::color color, const std::source_location &location) const;
    void log(const std::string &message, const char *category, fmt::color color) const;
    void logMsg(const std::string &formattedMessage) const;

    void               openLogFileIfNeeded();
    static void        openLogFile();
    static std::string formatLocation(const std::source_location &location);

    std::string                 title;
    inline static bool          logToFile = false;
    inline static std::ofstream logFile;
    inline static std::mutex    fileMutex;
    inline static fs::path      fileLocation = fs::current_path() / "log.txt";
};
