#pragma once

#include <filesystem>
#include <string>

#include "core.hpp"

BEGIN_XNOR_CORE

class Resource
{
public:
    explicit Resource(const std::string& filepath);

    virtual ~Resource() = default;

    Resource(const Resource& other) = delete;

    Resource(Resource&& other) = delete;

    Resource& operator=(const Resource& other) = delete;

    Resource& operator=(Resource&& other) = delete;

    virtual void Load() = 0;

    virtual void Unload();
    
    [[nodiscard]]
    const std::filesystem::path& GetFilepath() const;
    
    [[nodiscard]]
    const std::filesystem::path& GetFilename() const;
    
    [[nodiscard]]
    const std::filesystem::path& GetFilenameNoExtension() const;
    
private:
    std::filesystem::path m_Filepath;
    std::filesystem::path m_Filename;
    std::filesystem::path m_FilenameNoExtension;
};

END_XNOR_CORE
