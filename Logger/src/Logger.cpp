#include "Logger.h"

constexpr auto INFO_COLOR    = fmt::color::cyan;
constexpr auto DEBUG_COLOR   = fmt::color::green;
constexpr auto WARNING_COLOR = fmt::color::yellow;
constexpr auto ERROR_COLOR   = fmt::color::red;

Logger &Logger::Get()
{
    static Logger *instance = nullptr;
    if (!instance)
        instance = new Logger("");
    return *instance;
}
void Logger::LogToFile(const bool enable)
{
    std::lock_guard lock(fileMutex);
    logToFile = enable;
    if (logToFile && !logFile.is_open())
        openLogFile();
    else if (!logToFile && logFile.is_open())
        logFile.close();
}
void Logger::ClearLog()
{
    {
        std::lock_guard lock(fileMutex);
        if (logToFile)
        {
            if (logFile.is_open())
            {
                logFile.close();
            }
            fs::path logPath = fs::current_path().parent_path() / "log.txt";
            logFile.open(logPath, std::ios::out | std::ios::trunc);
            if (!logFile.is_open())
            {
                std::cerr << "Failed to clear log file" << std::endl;
                logToFile = false;
            }
        }
    }

#if defined(_WIN32) || defined(_WIN64)
    std::system("cls");
#else
    // NOTE: Ugly ass work around the gtk warnings
    std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
#endif
}
void Logger::Info(const std::string &message, const std::source_location &location) const
{
    log(message, "INFO", INFO_COLOR, location);
}
void Logger::InfoS(const std::string &message) const
{
    log(message, "INFO", INFO_COLOR);
}
void Logger::Debug(const std::string &message, const std::source_location &location) const
{
    log(message, "Debug", DEBUG_COLOR, location);
}
void Logger::DebugS(const std::string &message) const
{
    log(message, "Debug", DEBUG_COLOR);
}
void Logger::Warning(const std::string &message, const std::source_location &location) const
{
    log(message, "Warning", WARNING_COLOR, location);
}
void Logger::WarningS(const std::string &message) const
{
    log(message, "Warning", WARNING_COLOR);
}
void Logger::Error(const std::string &message, const std::source_location &location) const
{
    log(message, "ERROR", ERROR_COLOR, location);
}
void Logger::ErrorS(const std::string &message) const
{
    log(message, "Error", ERROR_COLOR);
}
void Logger::log(const std::string &message, const char *category, fmt::color color, const std::source_location &location) const
{
    std::string formattedMessage;
    if (!title.empty())
        formattedMessage = fmt::format("({}) [{}] {}: {}", title, formatLocation(location), category, message);
    else
        formattedMessage = fmt::format("[{}] {}: {}", formatLocation(location), category, message);

    fmt::print(fmt::fg(color), "{}: {}\n", category, message);

    if (logToFile)
    {
        std::lock_guard lock(fileMutex);
        if (logFile.is_open())
        {
            logFile << formattedMessage << std::endl;
            logFile.flush();
        }
    }
}
void Logger::logMsg(const std::string &formattedMessage) const
{
    if (logToFile)
    {
        std::lock_guard lock(fileMutex);
        if (logFile.is_open())
        {
            logFile << formattedMessage << std::endl;
            logFile.flush();
        }
    }
}
void Logger::log(const std::string &message, const char *category, fmt::color color) const
{
    std::string formattedMessage;
    if (!title.empty())
        formattedMessage = fmt::format("({}) {}: {}", title, category, message);
    else
        formattedMessage = fmt::format("{}: {}", category, message);

    fmt::print(fmt::fg(color), "{}: {}\n", category, message);

    logMsg(formattedMessage);
}
void Logger::openLogFileIfNeeded()
{
    std::lock_guard lock(fileMutex);
    if (logToFile && !logFile.is_open())
        openLogFile();
}
void Logger::openLogFile()
{
    // TODO: Ability to select where to place log
    static const fs::path logPath = fs::current_path() / "log.txt";
    logFile.open(logPath, std::ios::app);
    if (!logFile.is_open())
    {
        fmt::print(stderr, fmt::fg(DEBUG_COLOR), "Failed to open log file at {}", logPath.string());
        logToFile = false;
    }
}
std::string Logger::formatLocation(const std::source_location &location)
{
    static const fs::path currentDir   = fs::current_path().parent_path();
    std::string           relativePath = fs::relative(location.file_name(), currentDir).string();
    return relativePath + ":" + std::to_string(location.line()) + ":" + std::to_string(location.column());
}
