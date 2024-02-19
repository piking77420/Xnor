#include "windows/ui_window.hpp"

#include "ImGui/imgui.h"

bool UiWindow::IsFocused() const
{
    return m_IsFocused;
}

Vector2i UiWindow::GetPosition() const
{
    return m_Position;
}

Vector2i UiWindow::GetSize() const
{
    return m_Size;
}

void UiWindow::FetchInfo()
{
    m_IsFocused = ImGui::IsWindowFocused();

    const ImVec2 position = ImGui::GetWindowPos(); 
    m_Position = Vector2i(static_cast<int32_t>(position.x), static_cast<int32_t>(position.y));

    const ImVec2 size = ImGui::GetWindowSize(); 
    m_Size = Vector2i(static_cast<int32_t>(size.x), static_cast<int32_t>(size.y));
}
