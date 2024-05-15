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
void DebugConsole::DisplayHeader() const
{
    scroll = 0;
    if (ImGui::Button("Scroll up"))
        scroll = -1;
    ImGui::SameLine();
    if (ImGui::Button("Scroll down") || XnorCore::Logger::GetLogList().size() > m_LastLogCount)
        scroll = 1;
}

void DebugConsole::DisplayLogs()
{
    if (!ImGui::BeginTable("Logs", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY))
        return;

    // This will only be executed the first time the control flow reaches here
    static auto _ = [this] { return scroll = 1; }();
    
    auto logList = XnorCore::Logger::GetLogList();
    
    if (scroll == -1)
        ImGui::SetScrollHereY(0.f);
    
    ImGui::TableSetupColumn("Level");
    ImGui::TableSetupColumn("Timestamp");
    ImGui::TableSetupColumn("Message");
    ImGui::TableSetupScrollFreeze(0, 1);

    // Custom header for ComboBox in the Level header
    ImGui::TableNextRow(ImGuiTableRowFlags_Headers);

    // Level header
    ImGui::TableSetColumnIndex(0);
    ImGui::PushID(0);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    if (ImGui::BeginCombo("##level", magic_enum::enum_name(minimumLogLevel).data()))
    {
        for (auto&& level : magic_enum::enum_values<XnorCore::Logger::LogLevel>())
        {
            if (ImGui::Selectable(magic_enum::enum_name(level).data()))
            {
                minimumLogLevel = level;
                scroll = 1;
            }
        }
        ImGui::EndCombo();
    }
    ImGui::PopStyleVar();
    ImGui::SameLine();
    ImGui::Text("(%llu)", std::ranges::count_if(logList, [this] (auto&& val) -> bool_t { return val->level >= minimumLogLevel; }));
    ImGui::PopID();

    // Timestamp header
    ImGui::TableSetColumnIndex(1);
    ImGui::PushID(1);
    ImGui::TableHeader(ImGui::TableGetColumnName(1)); // Retrieve name passed to TableSetupColumn()
    ImGui::PopID();

    // Message header
    ImGui::TableSetColumnIndex(2);
    ImGui::PushID(2);
    ImGui::TableHeader(ImGui::TableGetColumnName(2)); // Retrieve name passed to TableSetupColumn()
    ImGui::PopID();
    
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

        std::string message;

        XnorCore::Color color;
        std::string prefix;
        switch (level)
        {
            case XnorCore::Logger::LogLevel::TemporaryDebug:
                color = XnorCore::Color::LightGreen();
                prefix = "Temporary Debug";
                message = log->file + '(' + std::to_string(log->line) + "): ";
                break;
                
            case XnorCore::Logger::LogLevel::Debug:
                color = XnorCore::Color::DarkGray();
                prefix = "Debug";
                break;
                
            case XnorCore::Logger::LogLevel::Info:
                color = XnorCore::Color::White();
                prefix = "Info";
                break;
                
            case XnorCore::Logger::LogLevel::Warning:
                color = XnorCore::Color::Orange();
                prefix = "Warning";
                break;
                
            case XnorCore::Logger::LogLevel::Error:
                color = XnorCore::Color::Red();
                prefix = "Error";
                break;
                
            case XnorCore::Logger::LogLevel::Fatal:
                color = XnorCore::Color::DarkRed();
                prefix = "Fatal";
                break;
        }

        if (oldSameLastLogs > 0)
            message = std::format("[...and {} more]", oldSameLastLogs);
        else
            message += log->message;

        const ImVec4 col = static_cast<Vector4>(color);

        ImGui::TableNextColumn();
        ImGui::TextColored(col, "%s", prefix.c_str());
        
        // Get the message time and format it in [hh:mm:ss:ms]
        const auto&& t = std::chrono::duration_cast<std::chrono::milliseconds, int64_t>(log->time.time_since_epoch());
        const std::string time = std::format("{:%T}", t);

        ImGui::TableNextColumn();
        ImGui::TextColored(col, "%s", time.c_str());
        
        ImGui::TableNextColumn();
        ImGui::PushTextWrapPos();
        ImGui::TextColored(col, "%s", message.c_str());
        ImGui::PopTextWrapPos();
    }

    if (scroll == 1)
        ImGui::SetScrollHereY(1.f);

    ImGui::EndTable();
}
