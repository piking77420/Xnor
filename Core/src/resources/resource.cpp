#include "resource.hpp"

using namespace XnorCore;

Resource::Resource(const std::string& filepath)
    : m_Filepath(filepath)
{
    if (!exists(m_Filepath))
        throw std::invalid_argument("Filepath does not exist");
    
    if (!is_regular_file(m_Filepath))
        throw std::invalid_argument("Filepath does not point to a file");
    
    m_Filename = m_Filepath.filename();
    m_FilenameNoExtension = m_Filepath.stem();
}

void Resource::Unload()
{
}

const std::filesystem::path& Resource::GetFilepath() const
{
    return m_Filepath;
}

const std::filesystem::path& Resource::GetFilename() const
{
    return m_Filename;
}

const std::filesystem::path& Resource::GetFilenameNoExtension() const
{
    return m_FilenameNoExtension;
}
