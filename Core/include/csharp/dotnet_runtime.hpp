#pragma once

#include <Coral/HostInstance.hpp>

#include "core.hpp"
#include "csharp/dotnet_assembly.hpp"
#include "utils/logger.hpp"

/// @file dotnet_runtime.hpp
/// @brief Defines the DotnetRuntime class.

BEGIN_XNOR_CORE

/// @brief Static class used to manage the current .NET runtime.
class DotnetRuntime
{
    STATIC_CLASS(DotnetRuntime)
    
public:
    /// @brief Initializes the .NET runtime.
    XNOR_ENGINE static bool_t Initialize();

    /// @brief Shutdowns the current .NET runtime.
    XNOR_ENGINE static void Shutdown();

    /// @brief 
    XNOR_ENGINE static bool_t LoadAssembly(const std::string& name);

    XNOR_ENGINE static DotnetAssembly* GetAssembly(const std::string& name);

    XNOR_ENGINE static void UnloadAllAssemblies(bool_t reloadContext = false);

    XNOR_ENGINE static void ReloadAllAssemblies();

    XNOR_ENGINE static bool_t BuildGameProject();

    XNOR_ENGINE static bool_t GetInitialized();

private:
    static inline constexpr int32_t DotnetMinVersionMajor = 5;
    static inline constexpr int32_t DotnetMinVersionMinor = 0;
    
    static Coral::HostSettings m_Settings;
    
    static inline Coral::HostInstance m_Runtime;
    
    static inline Coral::AssemblyLoadContext m_Alc;

    static inline bool_t m_Initialized = false;

    static inline std::vector<DotnetAssembly*> m_LoadedAssemblies;

    static bool CheckDotnetInstalled();

    static bool CheckDotnetVersion();

    static void CoralMessageCallback(std::string_view message, Coral::MessageLevel level);
    
    static void CoralExceptionCallback(std::string_view message);

    static Logger::LogLevel CoralMessageLevelToXnor(Coral::MessageLevel level);
};

END_XNOR_CORE
