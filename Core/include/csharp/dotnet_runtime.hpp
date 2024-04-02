#pragma once

#include <Coral/HostInstance.hpp>

#include "core.hpp"
#include "csharp/dotnet_assembly.hpp"
#include "utils/logger.hpp"

BEGIN_XNOR_CORE

class DotnetRuntime
{
    STATIC_CLASS(DotnetRuntime)
    
public:
    XNOR_ENGINE static bool_t Initialize();

    XNOR_ENGINE static void Shutdown();

    XNOR_ENGINE static bool_t LoadAssembly(const std::string& name);

    XNOR_ENGINE static DotnetAssembly* GetAssembly(const std::string& name);

    XNOR_ENGINE static void UnloadAllAssemblies(bool_t reloadContext = false);

    XNOR_ENGINE static void ReloadAllAssemblies();

    XNOR_ENGINE static bool_t BuildGameProject();

    XNOR_ENGINE static bool_t GetInitialized();

private:
    static inline constexpr int DotnetMinVersionMajor = 8;
    static inline constexpr int DotnetMinVersionMinor = 0;
    
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
