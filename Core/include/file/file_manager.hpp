#pragma once

#include <filesystem>
#include <map>

#include "core.hpp"
#include "file.hpp"
#include "utils/pointer.hpp"

BEGIN_XNOR_CORE

class FileManager final
{
public:
    // You cannot instantiate this class
    FileManager() = delete;
    
    [[nodiscard]]
    XNOR_ENGINE static Pointer<File> Load(std::filesystem::path filepath);

    [[nodiscard]]
    XNOR_ENGINE static bool Contains(const std::filesystem::path& filepath);

    [[nodiscard]]
    XNOR_ENGINE static Pointer<File> Get(const std::filesystem::path& filepath);

    XNOR_ENGINE static void Unload(const std::filesystem::path& filepath);

    XNOR_ENGINE static void Unload(const Pointer<File>& file);

    XNOR_ENGINE static void UnloadAll();
    
private:
    XNOR_ENGINE static inline std::map<std::filesystem::path, Pointer<File>> m_Files;
};

END_XNOR_CORE
