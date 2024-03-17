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
    static bool_t Initialize();

    static void Shutdown();

    static bool_t LoadAssembly(const std::string& filename);

    static void UnloadAllAssemblies(bool_t reloadContext = false);

    static void ReloadAllAssemblies();

    static bool_t GetInitialized();

private:
    static Coral::HostSettings m_Settings;
    
    static inline Coral::HostInstance m_Runtime;
    
    static inline Coral::AssemblyLoadContext m_Alc;

    static inline bool_t m_Initialized = false;

    static inline std::vector<DotnetAssembly*> m_LoadedAssemblies;

    static void CoralMessageCallback(std::string_view message, Coral::MessageLevel level);
    
    static void CoralExceptionCallback(std::string_view message);

    static Logger::LogLevel CoralMessageLevelToXnor(Coral::MessageLevel level);
};

END_XNOR_CORE
