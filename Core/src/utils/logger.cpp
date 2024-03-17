#include "utils/logger.hpp"

#include <fstream>
#include <iostream>

#include <windows.h>

#include "utils/formatter.hpp"

#define ANSI_COLOR_GRAY     "\x1b[38;5;242m"
#define ANSI_COLOR_GREEN    "\x1b[0;32m"
#define ANSI_COLOR_YELLOW   "\x1b[33m"
#define ANSI_COLOR_RED      "\x1b[31m"

#define ANSI_STYLE_BOLD     "\x1b[1m"

#define ANSI_RESET          "\x1b[0m"

using namespace XnorCore;

std::mutex mutex;
bool synchronizing = false;
bool running = true;
std::ofstream file;

void Logger::OpenFile(const std::filesystem::path &filepath)
{
    CloseFile();
    
    const bool exists = std::filesystem::exists(filepath);
    if (!exists)
        create_directories(filepath.parent_path());

    file.open(filepath, std::ios_base::out | std::ios_base::app);

    if (!file.is_open() || !file.good())
    {
        LogWarning("Could not open log file for writing: {}", absolute(filepath));
        return;
    }

    LogInfo("Logging to file: {}", filepath);

    // If the file already exists, add newlines to space from the last log
    if (!exists)
    {
        LogInfo("Starting logging #0");
    }
    else
    {
        // Write a newline to separate each log entry and use std::endl to make
        // sure to flush it so that when we count the number of newlines, we get
        // the correct number
        file << std::endl; // NOLINT(performance-avoid-endl)

        // Read file contents to count empty lines and therefore know how many logs
        // where written in the file.
        std::ifstream in(filepath);

        if (!in.is_open() || !in.good())
        {
            LogWarning("Could not open log file for reading: {}", absolute(filepath));
        }
        else
        {
            std::string line;
            uint32_t count = 0;
            while (!in.eof())
            {
                std::getline(in, line);
                if (line.empty() || line == "\n")
                    count++;
            }
            LogInfo("Starting logging #{}", 1);
        }
    }
}

void Logger::OpenDefaultFile()
{
    // Get the current date and format it in yyyy-mm-dd for the file name
    const std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm ltime;
    (void) localtime_s(&ltime, &t);
    const std::_Timeobj<char_t, const tm *> timeFormatter = std::put_time(&ltime, "%F.log");
    const std::string date = (std::ostringstream() << timeFormatter).str();
    OpenFile("logs/" + date);
}

bool Logger::HasFileOpen()
{
    return file.is_open();
}

void Logger::CloseFile()
{
    if (!file.is_open())
        return;
    
    file.flush();
    file.close();
}

void Logger::Synchronize()
{
    if (m_Lines.Empty())
        return;
    
    synchronizing = true;
    m_CondVar.notify_one();
    std::unique_lock lock(mutex);
    m_CondVar.wait(lock, [] { return !synchronizing; });
}

void Logger::Stop()
{
    LogInfo("Stopping logger");
    
    running = false;
    m_CondVar.notify_one();

    if (m_Thread.joinable())
        m_Thread.join();
}

Logger::LogEntry::LogEntry(std::string&& message, const LogLevel level)
    : LogEntry(
        std::move(message),
        level,
        std::chrono::zoned_time(std::chrono::current_zone(), std::chrono::floor<std::chrono::milliseconds>(std::chrono::system_clock::now()))
            .get_local_time().time_since_epoch()
    )
{
}

Logger::LogEntry::LogEntry(std::string&& message, const LogLevel level, const char_t* file, const int32_t line)
    : LogEntry(
        std::move(message),
        level,
        std::chrono::zoned_time(std::chrono::current_zone(), std::chrono::floor<std::chrono::milliseconds>(std::chrono::system_clock::now()))
            .get_local_time().time_since_epoch()
    )
{
    this->file = file;
    this->line = line;
}

Logger::LogEntry::LogEntry(
    std::string&& message,
    const LogLevel level,
    const std::chrono::system_clock::time_point timePoint
)
    : message(std::move(message))
    , level(level)
    , time(timePoint - std::chrono::floor<std::chrono::days>(timePoint))
    , printToConsole(level >= minimumConsoleLevel)
    , printToFile(level >= minimumFileLevel)
{
}

Logger::LogEntry::LogEntry(
    std::string&& message,
    const LogLevel level,
    const std::chrono::system_clock::duration duration
)
    : message(std::move(message))
    , level(level)
    , time(duration - std::chrono::floor<std::chrono::days>(duration))
    , printToConsole(level >= minimumConsoleLevel)
    , printToFile(level >= minimumFileLevel)
{
}

void Logger::Run()
{
    // Set thread name for easier debugging
    (void) SetThreadDescription(m_Thread.native_handle(), L"Logger Thread");
    
    std::unique_lock lock(mutex);
    while (running || !m_Lines.Empty())
    {
        m_CondVar.wait(lock, [] { return !m_Lines.Empty() || !running || synchronizing; });

        while (!m_Lines.Empty())
            PrintLog(m_Lines.Pop());

        // As we don't use std::endl for newlines, make sure to flush the streams before going to sleep
        std::cout.flush();
        if (file.is_open())
            file.flush();

        if (synchronizing)
        {
            synchronizing = false;
            m_CondVar.notify_one();
        }
    }
    
    CloseFile();
}

void Logger::PrintLog(const LogEntry& log)
{
    // Get the message time and format it in [hh:mm:ss:ms]
    const auto&& t = std::chrono::duration_cast<std::chrono::milliseconds, long long>(log.time.time_since_epoch());
    const std::string time = std::format("[{:%T}] ", t);

    // Setup the base text message
    std::string baseMessage = log.message + '\n';
    const LogLevel level = log.level;

    const char* color = ANSI_RESET;
    switch (level)
    {
        case LogLevel::TemporaryDebug:
            color = ANSI_COLOR_GREEN;
            baseMessage = time + "[TEMP DEBUG] " + log.file + "(" + std::to_string(log.line) + "): " + baseMessage;
            break;
        
        case LogLevel::Debug:
            color = ANSI_COLOR_GRAY;
            baseMessage = time + "[DEBUG] " + baseMessage;
            break;

        case LogLevel::Info:
            baseMessage = time + "[INFO] " + baseMessage;
            break;

        case LogLevel::Warning:
            color = ANSI_COLOR_YELLOW;
            baseMessage = time + "[WARN] " + baseMessage;
            break;

        case LogLevel::Error:
            color = ANSI_COLOR_RED;
            baseMessage = time + "[ERROR] " + baseMessage;
            break;

        case LogLevel::Fatal:
            color = ANSI_STYLE_BOLD ANSI_COLOR_RED;
            baseMessage = time + "[FATAL] " + baseMessage;
            break;
    }

    if (log.printToConsole)
        std::cout << color + baseMessage + ANSI_RESET;

    if (log.printToFile && file.is_open())
        file << baseMessage;
}

// Explicitly instantiate empty template versions of the Log functions for the C# DLL linkage
template void Logger::Log<>(LogLevel level, const std::string& format);
template void Logger::LogTempDebug<>(const std::string& format, const char_t* file, int32_t line);
template void Logger::LogDebug<>(const std::string& format);
template void Logger::LogInfo<>(const std::string& format);
template void Logger::LogWarning<>(const std::string& format);
template void Logger::LogError<>(const std::string& format);
template void Logger::LogFatal<>(const std::string& format);
