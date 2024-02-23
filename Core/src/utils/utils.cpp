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

std::string Utils::HumanizeString(const std::string& str)
{
    // Regex: https://regex101.com/r/3rQ25V/5
    // Matches any uppercase letter that has a lowercase variant,
    // that is not the first character in the string,
    // and that is either preceded or followed by a lowercase letter that has an uppercase variant
    static const std::regex Regex(R"((?:[a-z])([A-Z])|(?:.)([A-Z])(?:[a-z]))");

    // According to https://en.cppreference.com/w/cpp/string/byte/toupper,
    // when using the std::toupper function, to make sure the operation is executed
    // correctly, we should cast the input to unsigned char and the output to char
    const char_t firstCharUpper = static_cast<char_t>(std::toupper(static_cast<uint8_t>(str[0])));

    std::sregex_iterator begin(str.begin(), str.end(), Regex);
    std::sregex_iterator end;

    std::string result = firstCharUpper + str.substr(1);
    
    // Early return if nothing matches
    if (std::distance(begin, end) == 0)
        return result;

    const std::string::const_iterator& sBegin = str.begin();
    std::string::iterator rBegin = result.begin();
    for (std::sregex_iterator it = begin; it != end; ++it)
    {
        const std::smatch& match = *it;
        // Get results for capture groups 1 and 2
        for (int i = 1; i < 3; i++)
        {
            std::ssub_match subMatch = match[i];
            if (subMatch.matched)
            {
                result.replace(subMatch.first - sBegin + rBegin, subMatch.second - sBegin + rBegin, ' ' + subMatch.str());
                rBegin++;
            }
        }
    }

    return result;
}
