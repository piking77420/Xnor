#include "utils/utils.hpp"

#include <ImGui/imgui.h>

using namespace XnorCore;

void Utils::CenterImguiObject(const float alignment)
{
    const ImGuiStyle& style = ImGui::GetStyle();
    const float size = style.FramePadding.x * 2.0f;
    const float avail = ImGui::GetContentRegionAvail().x;
    const float off = (avail - size) * alignment;
    
    if (off > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
}
