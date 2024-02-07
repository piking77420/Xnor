#include "utils/logger.hpp"

#include <iostream>

#include <windows.h>

#define ANSI_COLOR_GRAY     "\x1b[38;5;242m"
#define ANSI_COLOR_YELLOW   "\x1b[33m"
#define ANSI_COLOR_RED      "\x1b[31m"

#define ANSI_STYLE_BOLD     "\x1b[1m"

#define ANSI_RESET          "\x1b[0m"

std::mutex mutex;
bool synchronizing = false;
bool running = true;
std::ofstream file;

void Logger::OpenFile(const std::filesystem::path &filename)
{
    CloseFile();
    
    const bool exists = std::filesystem::exists(filename);
    if (!exists)
        create_directories(filename.parent_path());

    file.open(filename, std::ios_base::out | std::ios_base::app);

    if (!file.is_open() || !file.good())
    {
        LogWarning("Could not open log file for writing: " + absolute(filename).string());
        return;
    }

    LogInfo("Logging to file: %s", filename.string().c_str());

    // If the file already exists, add newlines to space from the last log
    if (!exists)
        LogInfo("Starting logging #0");
    else
    {
        // Write a newline to separate each log entry and use std::endl to make
        // sure to flush it so that when we count the number of newlines, we get
        // the correct number
        file << std::endl;  // NOLINT(performance-avoid-endl)

        // Read file contents to count empty lines and therefore know how many logs
        // where written in the file.
        std::ifstream in(filename);

        if (!in.is_open() || !in.good())
            LogWarning("Could not open log file for reading: " + absolute(filename).string());
        else
        {
            std::string line;
            unsigned int count = 0;
            while (!in.eof())
            {
                std::getline(in, line);
                if (line.empty() || line == "\n")
                    count++;
            }
            LogInfo("Starting logging #%d", count - 1);
        }
    }
}

void Logger::OpenDefaultFile()
{
    // Get the current date and format it in yyyy-mm-dd for the file name
    const std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm ltime;
    (void) localtime_s(&ltime, &t);
    const std::_Timeobj<char, const tm *> timeFormatter = std::put_time(&ltime, "%F.log");
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
    if (lines.Empty())
        return;
    
    synchronizing = true;
    condVar.notify_one();
    std::unique_lock lock(mutex);
    condVar.wait(lock, [] { return !synchronizing; });
}

void Logger::Stop()
{
    Synchronize();
    
    running = false;
    condVar.notify_one();
    
    if (thread.joinable())
        thread.join();
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
    (void) SetThreadDescription(thread.native_handle(), L"Logger Thread");
    std::unique_lock lock(mutex);
    while (running || !lines.Empty())
    {
        condVar.wait(lock, [] { return !lines.Empty() || !running || synchronizing; });

        while (!lines.Empty())
            PrintLog(lines.Pop());

        // As we don't use std::endl for newlines, make sure to flush the streams before going to sleep
        std::cout.flush();
        if (file.is_open())
            file.flush();

        if (synchronizing)
        {
            synchronizing = false;
            condVar.notify_one();
        }
    }
    CloseFile();
}

void Logger::PrintLog(const LogEntry& entry)
{
    // Get the message time and format it in [hh:mm:ss:ms]
    const auto&& t = std::chrono::duration_cast<std::chrono::milliseconds, long long>(entry.time.time_since_epoch());
    const std::string time = std::format("[{:%T}] ", t);

    // Setup the base text message
    std::string baseMessage = entry.message + '\n';
    const LogLevel level = entry.level;

    const char* color = ANSI_RESET;
    switch (level)
    {
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

    if (entry.printToConsole)
        std::cout << color + baseMessage + ANSI_RESET;

    if (entry.printToFile && file.is_open())
        file << baseMessage;
}
