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

ImVec2 Utils::ToImVec(Vector2 v)
{
    return ImVec2(v.x, v.y);
}

Vector2 Utils::FromImVec(ImVec2 v)
{
    return Vector2(v.x, v.y);
}

std::string Utils::PathToForwardSlashes(const std::filesystem::path& path)
{
    return PathToForwardSlashes(path.string());
}

std::string Utils::PathToForwardSlashes(std::string path)
{
    std::ranges::replace(path, '\\', '/');
    
    return path;
}
