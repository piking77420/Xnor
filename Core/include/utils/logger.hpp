#pragma once

#include <condition_variable>
#include <filesystem>
#include <thread>

#include "core.hpp"
#include "utils/formatter.hpp"
#include "utils/ts_queue.hpp"

/// @file logger.hpp
/// @brief Defines the XnorCore::Logger static class

BEGIN_XNOR_CORE

/// @brief Logs a @ref XnorCore::Logger::LogLevel::TemporaryDebug "temporary debug message" using the given format message and arguments.
#define DEBUG_LOG(message, ...) XnorCore::Logger::LogTempDebug(message, __FILE__, __LINE__, __VA_ARGS__)

/// @brief Static class used to log messages to the console and/or a file.
///
/// ### Requirements
/// Thread-safe logger that starts logging even before @c main() gets called because of a static-storage thread. The only necessary thing
/// is to call Logger::Stop at the end of the program, which is already done in Application::~Application. You can synchronize the calling thread
/// with the logger one at any time by calling Logger::Synchronize.
///
/// ### Options
/// By default, the logger doesn't log to a file. This can be changed by either calling Logger::OpenDefaultFile or Logger::OpenFile.
/// You can also stop logging to the file whenever you want by calling Logger::CloseFile.
/// 
/// You can change at any time the minimum LogLevel for either the console or the file by respectively setting Logger::minimumConsoleLevel or Logger::minimumFileLevel
/// to a different value.
///
/// ### Usage
/// The most generic way of logging is by using the Logger::Log function, which allows you to pass a LogLevel to describe the severity
/// of the log. Shortcuts are also available through the use of Logger::LogTempDebug, Logger::LogDebug, Logger::LogInfo, Logger::LogWarning, Logger::LogError and Logger::LogFatal.
/// Those functions take a format string and format parameters to follow the usage of <a href="https://en.cppreference.com/w/cpp/utility/format/format">std::format</a>.
/// This means that any new parameter type that is directly printed must satisfy the requirements of the <a href="https://en.cppreference.com/w/cpp/utility/format/formattable">std::formattable</a>
/// concept (defined a Formattable in the XnorCore namespace), and therefore needs to implement its own version of the <a href="https://en.cppreference.com/w/cpp/utility/format/formatter">std::formatter</a> struct.
///
/// ### Example
/// All logs are preceded by their timestamp (the exact time at which the Logger::Log function was called), and a string representation of their LogLevel.
/// A typical log looks like the following:
/// @code
/// [11:26:05.751] [INFO] Starting logging to file.
/// @endcode
/// Also, @ref Logger::LogLevel::TemporaryDebug "temporary debug logs" automatically specify the file and line at which the log was made.
class Logger final
{
    STATIC_CLASS(Logger)
    
public:
    /// @brief Describes the severity of a log.
    enum class LogLevel
    {
        /// @brief Log intended for temporary debugging only.
        ///
        /// Preceded by '[TEMP DEBUG]' and appears green in the console.
        /// Temporary debug logs are not printed in the log file by default and they are only printed in the console if in a debug build.
        TemporaryDebug,
        /// @brief Log intended for debugging only.
        ///
        /// Preceded by '[DEBUG]' and appears gray in the console.
        /// Debug logs are not printed in the log file by default and they are only printed in the console if in a debug build.
        Debug,
        /// @brief Log intended for general information.
        ///
        /// Preceded by '[INFO]' and appears white in the console.
        /// Info logs are not printed in the log file by default.
        Info,
        /// @brief Log intended for warnings.
        ///
        /// Preceded by '[WARN]' and appears orange in the console.
        Warning,
        /// @brief Log intended for errors.
        ///
        /// Preceded by '[ERROR]' and appears red in the console.
        Error,
        /// @brief Log intended for fatal errors.
        ///
        /// Preceded by '[FATAL]' and appears red and bold in the console.
        /// After such a log, the program is not intended to continue to function normally and should instead exit ASAP.
        Fatal
    };

    /// @brief The minimum necessary LogLevel for a log to be printed in the console.
    ///
    /// Defaults to LogLevel::Debug in a debug build, or LogLevel::Info otherwise.
    XNOR_ENGINE static inline LogLevel minimumConsoleLevel =
#ifdef _DEBUG
        LogLevel::TemporaryDebug;
#else
        LogLevel::Info;
#endif
    
    /// @brief The minimum necessary LogLevel for a log to be printed in the log file.
    ///
    /// Defaults to LogLevel::Warning.
    XNOR_ENGINE static inline LogLevel minimumFileLevel = LogLevel::Warning;

    /// @brief Logs a message using the specified format string, arguments and LogLevel.
    /// 
    /// @tparam Args The format arguments types. These are generally automatically deducted by the compiler and often don't need to be manually specified. Also, they must satisfy the Formattable concept requirements.
    /// @param level The log severity.
    /// @param format The format string to log.
    /// @param args The arguments to replace the format string with.
    ///
    /// @see <a href="https://en.cppreference.com/w/cpp/utility/format/spec">The standard format specification</a>
    template <Formattable... Args>
    static void Log(LogLevel level, const std::string& format, Args&&... args);

    /// @brief Logs a temporary debug message using the current file, line, specified format string and arguments.
    ///
    /// This function shouldn't be used directly. To print a temporary debug log message, instead use DEBUG_LOG.
    ///
    /// @see Log
    /// @see LogLevel::TemporaryDebug
    template <Formattable... Args>
    static void LogTempDebug(const std::string& format, const char_t* file, int32_t line, Args&&... args);

    /// @brief Logs a debug message using the specified format string and arguments.
    ///
    /// @see Log
    template <Formattable... Args>
    static void LogDebug(const std::string& format, Args&&... args);

    /// @brief Logs a information message using the specified format string and arguments.
    ///
    /// @see Log
    template <Formattable... Args>
    static void LogInfo(const std::string& format, Args&&... args);

    /// @brief Logs a warning message using the specified format string and arguments.
    ///
    /// @see Log
    template <Formattable... Args>
    static void LogWarning(const std::string& format, Args&&... args);

    /// @brief Logs an error message using the specified format string and arguments.
    ///
    /// @see Log
    template <Formattable... Args>
    static void LogError(const std::string& format, Args&&... args);

    /// @brief Logs a fatal error message using the specified format string and arguments.
    ///
    /// @see Log
    template <Formattable... Args>
    static void LogFatal(const std::string& format, Args&&... args);

    /// @brief Opens a file for logging.
    ///
    /// If a file is already open for logging e.g. if Logger::HasFileOpen returns @c true, this overwrites it with the new file.
    ///
    /// @param filepath The file to open and to log into.
    XNOR_ENGINE static void OpenFile(const std::filesystem::path& filepath);

    /// @brief Opens the default log file.
    ///
    /// The default log file is an autogenerated one with a name corresponding to the current date and the @c .log file extension.
    /// An example of a default log file: @c 2024-02-13.log.
    /// If this is called multiple times in a day (by launching the program multiple times during the same day for example), this appends to the
    /// already existing log file.
    XNOR_ENGINE static void OpenDefaultFile();

    /// @brief Returns whether the logger is already log to a file.
    XNOR_ENGINE static bool HasFileOpen();

    /// @brief Closes the current log file.
    XNOR_ENGINE static void CloseFile();

    /// @brief Synchronizes the calling thread with the logger one, and makes sure all logs have been printed before returning.
    XNOR_ENGINE static void Synchronize();

    /// @brief Synchronizes the threads and stops the logger.
    ///
    /// After a call to this function, logger function calls won't do anything.
    XNOR_ENGINE static void Stop();

private:
    struct LogEntry
    {
        std::string message;
        LogLevel level;
        std::chrono::system_clock::time_point time;
        bool_t printToConsole, printToFile;
        const char_t* file = nullptr;
        int32_t line = -1;

        XNOR_ENGINE LogEntry(std::string&& message, LogLevel level);

        XNOR_ENGINE LogEntry(std::string&& message, LogLevel level, const char_t* file, int32_t line);

        XNOR_ENGINE LogEntry(std::string&& message, LogLevel level, std::chrono::system_clock::time_point timePoint);

        XNOR_ENGINE LogEntry(std::string&& message, LogLevel level, std::chrono::system_clock::duration duration);
    };

    XNOR_ENGINE static inline TsQueue<LogEntry> m_Lines;

    XNOR_ENGINE static inline std::condition_variable m_CondVar;

    XNOR_ENGINE static void Run();

    XNOR_ENGINE static inline std::thread m_Thread = std::thread(Run);

    static void PrintLog(const LogEntry& log);
};

END_XNOR_CORE

#include "utils/logger.inl"
