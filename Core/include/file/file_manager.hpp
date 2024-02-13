#pragma once

#include <filesystem>
#include <map>

#include "core.hpp"
#include "file.hpp"
#include "utils/pointer.hpp"

BEGIN_XNOR_CORE

class FileManager
{
public:
    // You cannot instantiate this class
    FileManager() = delete;
    
    [[nodiscard]]
    XNOR_ENGINE static Pointer<File> Load(std::filesystem::path filepath);

    XNOR_ENGINE static void Delete(const std::filesystem::path& filepath);

    XNOR_ENGINE static void Delete(const Pointer<File>& file);
    
private:
    static inline std::map<std::filesystem::path, Pointer<File>> m_Files;
};

END_XNOR_CORE
