#pragma once

#include "utils/logger.hpp"

#include <utility>

#include "utils/utils.hpp"

BEGIN_XNOR_CORE

template <Concepts::FormattableT... Args>
void Logger::Log(const LogLevel level, const std::string& format, Args&&... args)
{
    if (level < minimumConsoleLevel && level < minimumFileLevel)
        return;

    const std::shared_ptr<LogEntry> entry = std::make_shared<LogEntry>(std::vformat(format, std::make_format_args(std::forward<Args>(args)...)), level);
    
    m_Logs.Push(entry);
    if (m_LastLog && *m_LastLog == *entry)
        entry->previousLog = m_LastLog;
    else
        m_LastLog = entry;
    
    m_CondVar.notify_one();
}

template <Concepts::FormattableT ... Args>
void Logger::LogTempDebug(const std::string& format, const char_t* file, const int32_t line, Args&&... args)
{
    if (LogLevel::TemporaryDebug < minimumConsoleLevel && LogLevel::TemporaryDebug < minimumFileLevel)
        return;

    const std::shared_ptr<LogEntry> entry = std::make_shared<LogEntry>(std::vformat(format, std::make_format_args(std::forward<Args>(args)...)), LogLevel::TemporaryDebug, file, line);
    
    m_Logs.Push(entry);
    if (m_LastLog && *m_LastLog == *entry)
        entry->previousLog = m_LastLog;
    else
        m_LastLog = entry;
    
    m_CondVar.notify_one();
}

template <Concepts::FormattableT... Args>
void Logger::LogDebug(const std::string& format, Args&&... args)
{
    Logger::Log(LogLevel::Debug, format, std::forward<Args>(args)...);
}

template <Concepts::FormattableT... Args>
void Logger::LogInfo(const std::string& format, Args&&... args)
{
    Logger::Log(LogLevel::Info, format, std::forward<Args>(args)...);
}

template <Concepts::FormattableT... Args>
void Logger::LogWarning(const std::string& format, Args&&... args)
{
    Logger::Log(LogLevel::Warning, format, std::forward<Args>(args)...);
}

template <Concepts::FormattableT... Args>
void Logger::LogError(const std::string& format, Args&&... args)
{
    Logger::Log(LogLevel::Error, format, std::forward<Args>(args)...);
}

template <Concepts::FormattableT... Args>
void Logger::LogFatal(const std::string& format, Args&&... args)
{
    Logger::Log(LogLevel::Fatal, format, std::forward<Args>(args)...);
}

END_XNOR_CORE
