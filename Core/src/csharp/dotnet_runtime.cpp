#include "csharp/dotnet_runtime.hpp"

#include "file/file.hpp"

using namespace XnorCore;

constexpr const char* AlcName = "XNOR Coral AssemblyLoadContext";

Coral::HostSettings DotnetRuntime::m_Settings =
{
    .MessageCallback = CoralMessageCallback,
    .MessageFilter = Coral::MessageLevel::Warning,
    .ExceptionCallback = CoralExceptionCallback
};

bool_t DotnetRuntime::Initialize()
{
    Logger::LogDebug("Initializing .NET runtime");
    
    if (!m_Instance.Initialize(m_Settings))
    {
        Logger::LogError("An unknown error occured while initializing .NET runtime");
        return false;
    }

    m_Alc = m_Instance.CreateAssemblyLoadContext(AlcName);

    LoadAssembly("CoreCSharp.dll");

    return true;
}

void DotnetRuntime::Shutdown()
{
    Logger::LogDebug("Shutting down .NET runtime");

    if (!m_LoadedAssemblies.empty())
        UnloadAllAssemblies();
    if (m_Initialized)
        m_Instance.Shutdown();
}

void DotnetRuntime::LoadAssembly(const std::string& filename)
{
    Logger::LogInfo("Loading .NET assembly {}", filename);
    m_LoadedAssemblies.emplace_back(m_Alc.LoadAssembly(filename), filename);
}

void DotnetRuntime::UnloadAllAssemblies(const bool_t reloadContext)
{
    Logger::LogInfo("Unloading {} .NET assemblies", m_LoadedAssemblies.size());
    m_LoadedAssemblies.clear();
    m_Instance.UnloadAssemblyLoadContext(m_Alc);

    if (reloadContext)
        m_Alc = m_Instance.CreateAssemblyLoadContext(AlcName);
}

void DotnetRuntime::ReloadAllAssemblies()
{
    const decltype(m_LoadedAssemblies) assemblies = m_LoadedAssemblies;
    UnloadAllAssemblies();
    
    for (auto&& assembly : assemblies)
        LoadAssembly(assembly.filename);
}

bool_t DotnetRuntime::GetInitialized()
{
    return m_Initialized;
}

void DotnetRuntime::CoralMessageCallback(std::string_view message, const Coral::MessageLevel level)
{
    Logger::Log(CoralMessageLevelToXnor(level), "{}", message);
}

void DotnetRuntime::CoralExceptionCallback(std::string_view message)
{
    Logger::LogError("Unhandled C# exception: {}", message);
}

Logger::LogLevel DotnetRuntime::CoralMessageLevelToXnor(const Coral::MessageLevel level)
{
    switch (level)
    {
        case Coral::MessageLevel::Info:
            return Logger::LogLevel::Info;

        case Coral::MessageLevel::Warning:
            return Logger::LogLevel::Warning;

        case Coral::MessageLevel::Error:
        case Coral::MessageLevel::All:
            return Logger::LogLevel::Error;
    }

    // This should never happen
    throw std::runtime_error("Invalid Coral MessageLevel");
}
