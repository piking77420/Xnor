#include "file/file.hpp"

#include <fstream>

#include "utils/logger.hpp"

using namespace XnorCore;

File::File(std::filesystem::path&& filepath)
    : m_Filepath(std::move(filepath))
{
    if (!exists(m_Filepath))
        throw std::invalid_argument("Filepath does not exist");
    
    if (!is_regular_file(m_Filepath))
        throw std::invalid_argument("Filepath does not point to a file");
    
    m_Filename = m_Filepath.filename();
    m_FilenameNoExtension = m_Filepath.stem();
}

File::~File()
{
    if (m_Loaded)
        Unload();
}

bool File::Load()
{
    std::ifstream file(m_Filepath, std::ios::in | std::ios::ate | std::ios::binary);

    if (!file.is_open() || !file.good())
    {
        Logger::LogError("Couldn't open file for reading: {}", m_Filepath);
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

const std::filesystem::path& File::GetFilepath() const
{
    return m_Filepath;
}

const std::filesystem::path& File::GetFilename() const
{
    return m_Filename;
}

const std::filesystem::path& File::GetFilenameNoExtension() const
{
    return m_FilenameNoExtension;
}

bool File::IsLoaded() const
{
    return m_Loaded;
}

int64_t File::GetSize() const
{
    return m_Size;
}
