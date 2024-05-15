#pragma once

#include "definitions.hpp"
#include "utils/logger.hpp"
#include "windows/ui_window.hpp"

BEGIN_XNOR_EDITOR

class DebugConsole : public UiWindow
{
public:
    XnorCore::Logger::LogLevel minimumLogLevel = XnorCore::Logger::LogLevel::Info;
    
    explicit DebugConsole(Editor* editor);

    void Display() override;

private:
    size_t m_LastLogCount = 0;
    
    void DisplayHeader() const;

    void DisplayLogs();
};

END_XNOR_EDITOR
