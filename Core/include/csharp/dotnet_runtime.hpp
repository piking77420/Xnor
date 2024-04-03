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

    /// @brief Loads the assembly with the given @p name.
    ///
    /// This assembly should be in the working directory.
    ///
    /// @param name The name of the assembly to load. This should be the file name without the '.dll' extension.
    XNOR_ENGINE static bool_t LoadAssembly(const std::string& name);

    /// @brief Returns the loaded assembly with the given @p name.
    XNOR_ENGINE static DotnetAssembly* GetAssembly(const std::string& name);

    /// @brief Unloads all loaded assemblies.
    ///
    /// @param reloadContext Unloading all assemblies means unloading the AssemblyLoadContext. This parameter describes whether to reload the context afterwards.
    XNOR_ENGINE static void UnloadAllAssemblies(bool_t reloadContext = false);

    /// @brief Reloads all loaded assemblies.
    XNOR_ENGINE static void ReloadAllAssemblies();

    /// @brief Builds the Game .NET project.
    XNOR_ENGINE static bool_t BuildGameProject();

    /// @brief Returns whether the runtime is initialized.
    XNOR_ENGINE static bool_t GetInitialized();

private:
    XNOR_ENGINE static inline constexpr int32_t DotnetVersionMajor = 5;
    
    static Coral::HostSettings m_Settings;
    
    XNOR_ENGINE static inline Coral::HostInstance m_Runtime;
    
    XNOR_ENGINE static inline Coral::AssemblyLoadContext m_Alc;

    XNOR_ENGINE static inline bool_t m_Initialized = false;

    XNOR_ENGINE static inline std::vector<DotnetAssembly*> m_LoadedAssemblies;

    static bool CheckDotnetInstalled();

    static bool CheckDotnetVersion();

    static void CoralMessageCallback(std::string_view message, Coral::MessageLevel level);
    
    static void CoralExceptionCallback(std::string_view message);

    static Logger::LogLevel CoralMessageLevelToXnor(Coral::MessageLevel level);
};

END_XNOR_CORE
