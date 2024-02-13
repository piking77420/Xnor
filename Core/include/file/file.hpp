#pragma once

#include <filesystem>
#include <string>

#include "core.hpp"

BEGIN_XNOR_CORE

class File
{
public:
    [[nodiscard]]
    XNOR_ENGINE explicit File(std::filesystem::path&& filepath);

    /// @brief Returns <c>false</c> if an error occured while loading.
    XNOR_ENGINE bool Load();

    XNOR_ENGINE void Unload();
    
    [[nodiscard]]
    XNOR_ENGINE const std::filesystem::path& GetFilepath() const;
    
    [[nodiscard]]
    XNOR_ENGINE const std::filesystem::path& GetFilename() const;
    
    [[nodiscard]]
    XNOR_ENGINE const std::filesystem::path& GetFilenameNoExtension() const;
    
    [[nodiscard]]
    XNOR_ENGINE bool IsLoaded() const;

    template<typename T>
    [[nodiscard]]
    const T* GetData() const;

    template<typename T>
    [[nodiscard]]
    T* GetData();
    
    [[nodiscard]]
    XNOR_ENGINE int64_t GetSize() const;
    
private:
    std::filesystem::path m_Filepath;
    std::filesystem::path m_Filename;
    std::filesystem::path m_FilenameNoExtension;
    
    bool m_Loaded = false;
    int8_t* m_Data = nullptr;
    int64_t m_Size = 0;
};

template<typename T>
const T* File::GetData() const
{
    return reinterpret_cast<const T*>(m_Data);
}

template<typename T>
T* File::GetData()
{
    return reinterpret_cast<T*>(m_Data);
}

END_XNOR_CORE
