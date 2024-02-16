// ReSharper disable CppClangTidyCppcoreguidelinesMissingStdForward
#pragma once

#include <filesystem>
#include <thread>
#include <condition_variable>
#include <utility>

#include "core.hpp"
#include "utils/tsqueue.hpp"

BEGIN_XNOR_CORE

class Logger
{
public:
    enum class LogLevel : unsigned char
    {
        Debug,
        Info,
        Warning,
        Error,
        Fatal
    };

    static inline LogLevel minimumConsoleLevel =
#ifdef _DEBUG
        LogLevel::Debug;
#else
        LogLevel::Info;
#endif
    
    static inline LogLevel minimumFileLevel = LogLevel::Warning;

    // You cannot instantiate this class
    Logger() = delete;

    template<class... Args>
    static void Log(LogLevel level, const std::string& format, Args&&... args);

    template<class... Args>
    static void LogDebug(const std::string& format, Args&&... args);

    template<class... Args>
    static void LogInfo(const std::string& format, Args&&... args);

    template<class... Args>
    static void LogWarning(const std::string& format, Args&&... args);

    template<class... Args>
    static void LogError(const std::string& format, Args&&... args);

    template<class... Args>
    static void LogFatal(const std::string& format, Args&&... args);

    static void OpenFile(const std::filesystem::path& filename);

    XNOR_ENGINE static void OpenDefaultFile();

    XNOR_ENGINE static bool HasFileOpen();

    XNOR_ENGINE static void CloseFile();

    XNOR_ENGINE static void Synchronize();

    XNOR_ENGINE static void Stop();

private:
    struct LogEntry
    {
        std::string message;
        LogLevel level;
        std::chrono::system_clock::time_point time;
        bool printToConsole, printToFile;

        XNOR_ENGINE LogEntry(std::string&& message, LogLevel level);

        XNOR_ENGINE LogEntry(std::string&& message, LogLevel level, std::chrono::system_clock::time_point timePoint);

        XNOR_ENGINE LogEntry(std::string&& message, LogLevel level, std::chrono::system_clock::duration duration);
    };

    XNOR_ENGINE static inline TsQueue<LogEntry> m_Lines;

    XNOR_ENGINE static inline std::condition_variable m_CondVar;

    XNOR_ENGINE static void Run();

    XNOR_ENGINE static inline std::thread m_Thread = std::thread(&Logger::Run);

    static void PrintLog(const LogEntry& entry);

    template<class... Args>
    static std::string Format(const std::string& format, Args&&... args);
};

template class TsQueue<Logger::LogEntry>;

template<class... Args>
void Logger::Log(const LogLevel level, const std::string& format, Args&&... args)
{
    if (level < minimumConsoleLevel && level < minimumFileLevel)
        return;

    m_Lines.Push(LogEntry(Logger::Format(format, std::forward<Args>(args)...), level));
    m_CondVar.notify_one();
}

template<class... Args>
void Logger::LogDebug(const std::string& format, Args&&... args)
{
    Logger::Log(LogLevel::Debug, format, std::forward<Args>(args)...);
}

template<class... Args>
void Logger::LogInfo(const std::string& format, Args&&... args)
{
    Logger::Log(LogLevel::Info, format, std::forward<Args>(args)...);
}

template<class... Args>
void Logger::LogWarning(const std::string& format, Args&&... args)
{
    Logger::Log(LogLevel::Warning, format, std::forward<Args>(args)...);
}

template<class... Args>
void Logger::LogError(const std::string& format, Args&&... args)
{
    Logger::Log(LogLevel::Error, format, std::forward<Args>(args)...);
}

template<class... Args>
void Logger::LogFatal(const std::string& format, Args&&... args)
{
    Logger::Log(LogLevel::Fatal, format, std::forward<Args>(args)...);
}

template<class... Args>
std::string Logger::Format(const std::string& format, Args&&... args)
{
    char buffer[0x800];
    (void)sprintf_s(buffer, sizeof(buffer), format.c_str(), std::forward<Args>(args)...);
    return buffer;
}

END_XNOR_CORE
