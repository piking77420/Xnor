#include "file/file.hpp"

#include <fstream>

#include "resource/resource.hpp"
#include "utils/formatter.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

File::File(std::filesystem::path&& filepath)
    : Entry(std::move(filepath))
{
    if (!is_regular_file(m_Path))
        throw std::invalid_argument("Path does not point to a file");

    File::UpdateUtilityValues();
}

File::~File()
{
    if (m_Loaded)
        File::Unload();
}

bool_t File::Load()
{
    std::ifstream file(m_Path, std::ios::in | std::ios::ate | std::ios::binary);

    if (!file.is_open() || !file.good())
    {
        Logger::LogError("Couldn't open file for reading: {}", m_Path);
        return false;
    }

    m_Size = file.tellg();
    file.seekg(0);

    m_Data = new int8_t[m_Size];

    file.read(reinterpret_cast<char*>(m_Data), m_Size);
    
    m_Loaded = true;
    return true;
}

void File::Unload()
{
    delete[] m_Data;
    m_Size = 0;
    
    m_Loaded = false;
}

std::string File::GetNameNoExtension() const
{
    return m_NameNoExtension;
}

std::string File::GetExtension() const
{
    return m_Extension;
}

int64_t File::GetSize() const
{
    return m_Size;
}

void File::SetName(const std::string& newName)
{
    Entry::SetName(newName);

    m_Resource->SetName(GetPathString());
}

Pointer<Resource> File::GetResource() const
{
    return m_Resource;
}

void File::UpdateUtilityValues()
{
    Entry::UpdateUtilityValues();
    
    m_NameNoExtension = m_Path.stem().generic_string();
    m_Extension = m_Path.extension().string();
}
