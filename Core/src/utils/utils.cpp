#include "utils/utils.hpp"

#include <regex>
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

ImVec2 Utils::ToImVec(const Vector2 v)
{
    return ImVec2(v.x, v.y);
}

Vector2 Utils::FromImVec(const ImVec2 v)
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

std::string Utils::HumanizeString(const std::string& str)
{
    // Regex: https://regex101.com/r/3rQ25V/1
    // Matches any uppercase letter that has a lowercase variant,
    // that is not the first character in the string,
    // and that is either preceded or followed by a lowercase letter that has an uppercase variant
    const std::regex regex(R"(?:(?<=\p{Ll})\p{Lu})|(?:\p{Lu}(?=\p{Ll}))(?<!^.)");

    const std::string format(
        "$`"   // $` means characters before the match
        " $&"  // $& means the matched characters
        "$'"   // $' means characters following the match
    );
    
    std::string result;
    std::regex_replace(result.begin(), str.begin(), str.end(), regex, format);

    // According to https://en.cppreference.com/w/cpp/string/byte/toupper,
    // when using the std::toupper function, to make sure the operation is executed
    // correctly, we should cast the input to unsigned char and the output to char
    return static_cast<char>(std::toupper(static_cast<uint8_t>(str[0]))) + str.substr(1);
}

constexpr size_t Utils::GetNextPowerOfTwo(const size_t value)
{
    return std::bit_ceil(value);
}
