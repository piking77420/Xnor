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
bool_t synchronizing = false;
bool_t running = true;
std::ofstream file;

uint32_t logIndex = 0;

constexpr const char_t* const LogBegin = "# LOG {} BEGIN";
constexpr const char_t* const LogEnd = "# LOG {} END";

void Logger::OpenFile(const std::filesystem::path &filepath)
{
    CloseFile();
    
    const bool_t exists = std::filesystem::exists(filepath);
    if (!exists)
    {
        create_directories(filepath.parent_path());
    }
    else
    {
        // Read file contents to get the current file's log count
        std::ifstream in(filepath, std::ios_base::in | std::ios_base::ate);

        if (!in.is_open() || !in.good())
        {
            LogWarning("Could not open log file for reading: {}", absolute(filepath));
        }
        else
        {
            in.seekg(-1, decltype(in)::cur);
            std::string line;
            std::getline(in, line);
            
            std::string str = LogBegin;
            std::stringstream s(line);
            s.seekg(static_cast<std::streamoff>(str.find('{')));
            s >> logIndex;
        }
    }

    file.open(filepath, std::ios_base::out | std::ios_base::ate);

    if (!file.is_open() || !file.good())
    {
        LogWarning("Could not open log file for writing: {}", absolute(filepath));
        return;
    }
    
    // Separate the current logs from the previous ones using a newline
    if (exists)
        file << '\n';

    file << std::format(LogBegin, logIndex) << '\n';

    LogInfo("Logging to file: {}", filepath);
    // Prevent this log from being printed to the file
    m_Lines.Back().printToFile = false;

    LogInfo("Starting logging #{}", logIndex);
}

void Logger::OpenDefaultFile()
{
    // Get the current date and format it in yyyy-mm-dd for the file name
    const std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm localTime{};
    (void) localtime_s(&localTime, &t);
    const std::_Timeobj<char_t, const tm *> timeFormatter = std::put_time(&localTime, "%F.log");
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

    file << std::format(LogEnd, logIndex) << '\n';
    
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

Logger::LogEntry::LogEntry(std::string&& message, const LogLevel level)
    : LogEntry(
        std::move(message),
        level,
        std::chrono::zoned_time(std::chrono::current_zone(), std::chrono::floor<std::chrono::milliseconds>(std::chrono::system_clock::now()))
            .get_local_time().time_since_epoch()
    )
{
}

Logger::LogEntry::LogEntry(std::string&& message, const LogLevel level, const std::string& file, const int32_t line)
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

    if (std::atexit(Stop))
        LogWarning("Couldn't register Logger::Stop using std::atexit");
    
    std::unique_lock lock(mutex);
    while (running || !m_Lines.Empty())
    {
        m_CondVar.wait(lock, [] { return !m_Lines.Empty() || !running || synchronizing; });

        while (!m_Lines.Empty())
            PrintLog(m_Lines.Pop());

        // As we don't use std::endl for newlines, make sure to flush the streams before going back to sleep
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

    // Set up the base text message
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

void Logger::Stop()
{
    LogInfo("Stopping logger");
    
    running = false;
    m_CondVar.notify_one();

    if (m_Thread.joinable())
        m_Thread.join();
}
