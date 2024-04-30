#include "windows/debug_console.hpp"

#include "magic_enum/magic_enum.hpp"

using namespace XnorEditor;

DebugConsole::DebugConsole(Editor* editor)
    : UiWindow(editor, "Console")
{
}

void DebugConsole::Display()
{
    DisplayHeader();
    DisplayLogs();
    
    m_LastLogCount = XnorCore::Logger::GetLogList().size();
}

int scroll;
void DebugConsole::DisplayHeader()
{
    if (ImGui::BeginCombo("Log level", magic_enum::enum_name(minimumLogLevel).data()))
    {
        for (auto&& level : magic_enum::enum_values<XnorCore::Logger::LogLevel>())
        {
            if (ImGui::Selectable(magic_enum::enum_name(level).data()))
                minimumLogLevel = level;
        }
        ImGui::EndCombo();
    }

    scroll = 0;
    if (ImGui::Button("Scroll up"))
        scroll = -1;
    ImGui::SameLine();
    if (ImGui::Button("Scroll down") || XnorCore::Logger::GetLogList().size() > m_LastLogCount || ImGui::IsWindowAppearing())
        scroll = 1;
}

void DebugConsole::DisplayLogs() const
{
    ImGui::BeginChild("Logs", Vector2(-1.f), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY);

    if (scroll == -1)
        ImGui::SetScrollHereY(0.f);

    auto logList = XnorCore::Logger::GetLogList();
    for (decltype(logList)::const_iterator it = logList.cbegin(); it != logList.cend(); it++)
    {
        const auto& log = *it;
        
        const XnorCore::Logger::LogLevel level = log->level;
        if (level < minimumLogLevel)
            continue;

        static uint64_t sameLastLogs;
        static uint64_t oldSameLastLogs;

        oldSameLastLogs = sameLastLogs;
        if (log->sameAsPrevious)
            sameLastLogs++;
        else
            sameLastLogs = 0;

        if (sameLastLogs > 0 && it + 1 != logList.cend())
            continue;

        std::string message = log->message;

        XnorCore::Color color;
        std::string prefix;
        switch (level)
        {
            case XnorCore::Logger::LogLevel::TemporaryDebug:
                color = XnorCore::Color::LightGreen();
                prefix = "Temporary Debug:";
                message = log->file + '(' + std::to_string(log->line) + "): " + message;
                break;
                
            case XnorCore::Logger::LogLevel::Debug:
                color = XnorCore::Color::DarkGray();
                prefix = "Debug:";
                break;
                
            case XnorCore::Logger::LogLevel::Info:
                color = XnorCore::Color::White();
                prefix = "Info:";
                break;
                
            case XnorCore::Logger::LogLevel::Warning:
                color = XnorCore::Color::Orange();
                prefix = "Warning:";
                break;
                
            case XnorCore::Logger::LogLevel::Error:
                color = XnorCore::Color::Red();
                prefix = "Error:";
                break;
                
            case XnorCore::Logger::LogLevel::Fatal:
                color = XnorCore::Color::DarkRed();
                prefix = "Fatal:";
                break;
        }

        if (oldSameLastLogs > 0)
            message = std::format("[...and {} more]", oldSameLastLogs);

        ImGui::TextColored(static_cast<Vector4>(color), "%s %s", prefix.c_str(), message.c_str());
    }
    
    if (scroll == 1)
        ImGui::SetScrollHereY(1.f);

    ImGui::EndChild();
}
