#include "utils/logger.hpp"

#include <iostream>

#include "utils/formatter.hpp"
#include "utils/windows.hpp"

#define ANSI_COLOR_GRAY     "\x1b[38;5;242m"
#define ANSI_COLOR_GREEN    "\x1b[0;32m"
#define ANSI_COLOR_YELLOW   "\x1b[33m"
#define ANSI_COLOR_RED      "\x1b[31m"

#define ANSI_STYLE_BOLD     "\x1b[1m"

#define ANSI_RESET          "\x1b[0m"

using namespace XnorCore;

void Logger::OpenFile(const std::filesystem::path &filepath)
{
    CloseFile();
    
    const bool_t exists = std::filesystem::exists(filepath);
    if (!exists)
    {
        create_directories(filepath.parent_path());
    }

    m_File.open(filepath, std::ios_base::out | std::ios_base::app);
    m_Filepath = filepath;

    if (!m_File.is_open() || !m_File.good())
    {
        LogWarning("Could not open log file for writing: {}", absolute(filepath));
        return;
    }
    
    // Separate the current logs from the previous ones using a newline
    if (exists)
        m_File << '\n';

    LogInfo("Logging to file: {}", filepath);
    // Prevent this log from being printed to the file
    m_Logs.Back()->printToFile = false;

    LogInfo("Starting logging #{}", m_LogIndex);
}

void Logger::OpenDefaultFile()
{
    // Get the current date and format it in yyyy-mm-dd for the file name
    const std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm localTime{};
    (void) localtime_s(&localTime, &t);
    const std::_Timeobj<char_t, const tm *> timeFormatter = std::put_time(&localTime, "%F");
    const std::string date = (std::ostringstream() << timeFormatter).str();

    const std::filesystem::path directory = std::filesystem::path("logs") / date;
    if (!exists(directory))
    {
        OpenFile(directory / "0.log");
        return;
    }

    // Count the number of existing logs to get the log index
    // Start at -1 so that we get index-like numbers, e.g. 0 for the first one, 1 for the second one, etc...
    int32_t fileCount = -1;
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(directory))
    {
        if (is_regular_file(entry))
            fileCount++;
    }

    m_LogIndex = fileCount;
    OpenFile(directory / (std::to_string(fileCount) + ".log"));
}

bool Logger::HasFileOpen()
{
    return m_File.is_open();
}

void Logger::CloseFile()
{
    if (!m_File.is_open())
        return;
    
    m_File.flush();
    m_File.close();

    copy_file(m_Filepath, m_Filepath.parent_path() / "latest.log", std::filesystem::copy_options::overwrite_existing);
}

void Logger::Synchronize()
{
    if (m_Logs.Empty())
        return;
    
    m_Synchronizing = true;
    m_CondVar.notify_one();
    std::unique_lock lock(m_Mutex);
    m_CondVar.wait(lock, [] { return !m_Synchronizing; });
}

Logger::LogEntry::LogEntry()
    : level(LogLevel::Info)
    , printToConsole(false)
    , printToFile(false)
{
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

bool_t Logger::LogEntry::operator==(const LogEntry& other) const { return message == other.message && level == other.level; }

void Logger::Run()
{
    // Set thread name for easier debugging
    (void) SetThreadDescription(m_Thread.native_handle(), L"Logger Thread");

    if (std::atexit(Stop))
        LogWarning("Couldn't register Logger::Stop using std::atexit");
    
    std::unique_lock lock(m_Mutex);
    while (m_Running || !m_Logs.Empty())
    {
        m_CondVar.wait(lock, [] { return !m_Logs.Empty() || !m_Running || m_Synchronizing; });

        while (!m_Logs.Empty())
            PrintLog(m_Logs.Pop());

        // As we don't use std::endl for newlines, make sure to flush the streams before going back to sleep
        std::cout.flush();
        if (m_File.is_open())
            m_File.flush();

        if (m_Synchronizing)
        {
            m_Synchronizing = false;
            m_CondVar.notify_one();
        }
    }
}

void Logger::PrintLog(const std::shared_ptr<LogEntry>& log)
{
    // Get the message time and format it in [hh:mm:ss:ms]
    const auto&& t = std::chrono::duration_cast<std::chrono::milliseconds, int64_t>(log->time.time_since_epoch());
    const std::string time = std::format("[{:%T}] ", t);

    std::string baseMessage;
    const LogLevel level = log->level;
    const char_t* color = ANSI_RESET;
    switch (level)
    {
        case LogLevel::TemporaryDebug:
            color = ANSI_COLOR_GREEN;
            baseMessage = time + "[TEMP DEBUG] " + log->file + "(" + std::to_string(log->line) + "): ";
            break;
        
        case LogLevel::Debug:
            color = ANSI_COLOR_GRAY;
            baseMessage = time + "[DEBUG] ";
            break;

        case LogLevel::Info:
            baseMessage = time + "[INFO] ";
            break;

        case LogLevel::Warning:
            color = ANSI_COLOR_YELLOW;
            baseMessage = time + "[WARN] ";
            break;

        case LogLevel::Error:
            color = ANSI_COLOR_RED;
            baseMessage = time + "[ERROR] ";
            break;

        case LogLevel::Fatal:
            color = ANSI_STYLE_BOLD ANSI_COLOR_RED;
            baseMessage = time + "[FATAL] ";
            break;
    }

    baseMessage += log->message + '\n';

    if (log->printToConsole)
        std::cout << color + baseMessage + ANSI_RESET;

    if (log->printToFile && m_File.is_open())
        m_File << baseMessage;
}

void Logger::Stop()
{
    LogInfo("Stopping logger");
    
    m_Running = false;
    m_CondVar.notify_one();

    if (m_Thread.joinable())
        m_Thread.join();
    
    CloseFile();
}
