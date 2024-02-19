#include "utils/utils.hpp"

#include <ImGui/imgui.h>

void XnorCore::Utils::CenterImguiObject(float alignment)
{
    ImGuiStyle& style = ImGui::GetStyle();
    float size = style.FramePadding.x * 2.0f;
    float avail = ImGui::GetContentRegionAvail().x;

    float off = (avail - size) * alignment;
    if (off > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
}
