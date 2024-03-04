#include "utils/utils.hpp"

#include <regex>
#include <ImGui/imgui.h>

#include "file/directory.hpp"
#include "file/entry.hpp"
#include "file/file.hpp"

using namespace XnorCore;

void Utils::AlignImGuiCursor(const float_t objectWidth, const float_t alignment)
{
    const float_t avail = ImGui::GetContentRegionAvail().x;
    const float_t off = avail * alignment - objectWidth / 2.f;
    
    if (off > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
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

float_t Utils::NormalizeAngle(float_t angle)
{
    while (angle > Calc::PiOver2)
        angle -= Calc::PiOver2;
        
    while (angle < 0)
        angle += Calc::PiOver2;
        
    return angle;
}

Vector3 Utils::NormalizeAngles(const Vector3 angles)
{
    const Vector3 normalized = Vector3(
        NormalizeAngle(angles.x),
        NormalizeAngle(angles.y),
        NormalizeAngle(angles.z)
    );
    
    return normalized;
}

Vector3 Utils::GetQuaternionEulerAngles(const Quaternion& rot)
{
    const float_t sqw = rot.W() * rot.W();
    const float_t sqx = rot.X() * rot.X();
    const float_t sqy = rot.Y() * rot.Y();
    const float_t sqz = rot.Z() * rot.Z();
    const float_t unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
    const float_t test = rot.X() * rot.W() - rot.Y() * rot.Z();
    Vector3 v;
 
    if (test > 0.4995f * unit)
    { // singularity at north pole
        v.y = 2.f * std::atan2f(rot.Y(), rot.X());
        v.x = Calc::PiOver2;
        v.z = 0;
        return NormalizeAngles(v);
    }
    if (test < -0.4995f * unit)
    { // singularity at south pole
        v.y = -2.f * std::atan2f(rot.Y(), rot.X());
        v.x = -Calc::PiOver2;
        v.z = 0;
        return NormalizeAngles(v);
    }
 
    Quaternion q = Quaternion(rot.Z(), rot.X(), rot.Y(), rot.W());
    v.y = std::atan2f(2.f * q.X() * q.W() + 2.f * q.Y() * q.Z(), 1 - 2.f * (q.Z() * q.Z() + q.W() * q.W()));    // Yaw
    v.x = std::asinf(2.f * (q.X() * q.Z() - q.W() * q.Y()));                                                    // Pitch
    v.z = std::atan2f(2.f * q.X() * q.Y() + 2.f * q.Z() * q.W(), 1 - 2.f * (q.Y() * q.Y() + q.Z() * q.Z()));    // Roll
    return NormalizeAngles(v);
}

void Utils::OpenInExplorer(const Entry& entry)
{
    OpenInExplorer(entry.GetPath());
}

void Utils::OpenInExplorer(const Directory& directory)
{
    OpenInExplorer(directory.GetPath(), false);
}

void Utils::OpenInExplorer(const File& file)
{
    OpenInExplorer(file.GetPath(), true);
}

void Utils::OpenInExplorer(const std::filesystem::path& path)
{
    OpenInExplorer(path, !is_directory(path));
}

void Utils::OpenInExplorer(const std::filesystem::path& path, const bool isFile)
{
    std::string command = "start explorer ";
    
    if (isFile)
        command += "/select,";
    
    command += absolute(path).string();
    std::system(command.c_str());  // NOLINT(concurrency-mt-unsafe)
}

void Utils::OpenFile(const File& file)
{
    OpenFile(file.GetPath());
}

void Utils::OpenFile(const std::filesystem::path& filepath)
{
    std::string command = "start explorer ";
    command += absolute(filepath).string();
    std::system(command.c_str());  // NOLINT(concurrency-mt-unsafe)
}
