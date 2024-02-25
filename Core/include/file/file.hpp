#pragma once

#include <filesystem>

#include "core.hpp"
#include "file/entry.hpp"

BEGIN_XNOR_CORE
class File : public Entry
{
public:
    [[nodiscard]]
    XNOR_ENGINE explicit File(std::filesystem::path&& filepath);

    XNOR_ENGINE ~File() override;

    DEFAULT_COPY_MOVE_OPERATIONS(File)

    XNOR_ENGINE bool Load() override;

    XNOR_ENGINE void Unload() override;
    
    [[nodiscard]]
    XNOR_ENGINE std::string GetNameNoExtension() const;
    
    [[nodiscard]]
    XNOR_ENGINE std::string GetExtension() const;

    template<typename T = char_t>
    [[nodiscard]]
    const T* GetData() const;

    template<typename T = char_t>
    [[nodiscard]]
    T* GetData();
    
    [[nodiscard]]
    XNOR_ENGINE int64_t GetSize() const;
    
private:
    std::string m_NameNoExtension;
    std::string m_Extension;
    
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
