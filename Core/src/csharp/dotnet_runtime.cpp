#include "csharp/dotnet_runtime.hpp"

#include <cstdio>

#include "file/file.hpp"

#include "application.hpp"

// We need to include Window.h here but it must be the last include as it breaks everything otherwise
#undef APIENTRY
#include <Windows.h>

#include "reflection/dotnet_reflection.hpp"

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
    Logger::LogInfo("Initializing .NET runtime");

    if (!CheckDotnetInstalled())
    {
        Logger::LogError(".NET is not installed on this machine");
        throw std::runtime_error(".NET is not installed on this machine");
    }

    if (!CheckDotnetVersion())
    {
        Logger::LogError("Invalid .NET version. The XNOR Engine needs at least .NET Core {}.{}", DotnetMinVersionMajor, DotnetMinVersionMinor);
        throw std::runtime_error("Invalid .NET version");
    }

    m_Settings.CoralDirectory = Application::executablePath.parent_path().string();
    
    if (!m_Runtime.Initialize(m_Settings))
    {
        Logger::LogError("An unknown error occured while initializing .NET runtime");
        return false;
    }

    m_Alc = m_Runtime.CreateAssemblyLoadContext(AlcName);

    LoadAssembly("CoreCSharp");

    return true;
}

void DotnetRuntime::Shutdown()
{
    Logger::LogInfo("Shutting down .NET runtime");

    if (!m_LoadedAssemblies.empty())
        UnloadAllAssemblies();
    if (m_Initialized)
        m_Runtime.Shutdown();
}

bool_t DotnetRuntime::LoadAssembly(const std::string& name)
{
    const std::filesystem::path&& filepath = Application::executablePath.parent_path().string() + static_cast<char_t>(std::filesystem::path::preferred_separator) + name + ".dll";
    
    Logger::LogInfo("Loading .NET assembly {}", filepath.filename());

    const std::string&& str = filepath.string();
    
    DotnetAssembly* const assembly = new DotnetAssembly(str);
    if (assembly->Load(m_Alc))
    {
        //assembly->ProcessTypes();
        m_LoadedAssemblies.push_back(assembly);

        if (name == "Game")
            DotnetReflection::PrintTypes();
    }

    return false;
}

DotnetAssembly* DotnetRuntime::GetAssembly(const std::string& name)
{
    for (auto&& assembly : m_LoadedAssemblies)
    {
        if (assembly->GetName() == name)
            return assembly;
    }

    return nullptr;
}

void DotnetRuntime::UnloadAllAssemblies(const bool_t reloadContext)
{
    Logger::LogInfo("Unloading {} .NET assemblies", m_LoadedAssemblies.size());
    
    for (auto&& assembly : m_LoadedAssemblies)
        delete assembly;
    m_LoadedAssemblies.clear();
    
    m_Runtime.UnloadAssemblyLoadContext(m_Alc);

    if (reloadContext)
        m_Alc = m_Runtime.CreateAssemblyLoadContext(AlcName);
}

void DotnetRuntime::ReloadAllAssemblies()
{
    std::vector<std::string> assemblies;
    std::ranges::transform(m_LoadedAssemblies, std::back_inserter(assemblies), [](const decltype(m_LoadedAssemblies)::value_type& loadedAssembly) { return loadedAssembly->GetName(); });
    UnloadAllAssemblies(true);
    
    for (auto&& assembly : assemblies)
        LoadAssembly(assembly);
}

bool_t DotnetRuntime::GetInitialized()
{
    return m_Initialized;
}

bool DotnetRuntime::CheckDotnetInstalled()
{
    // Check if the dotnet command returns a non-zero exit code
    return std::system("dotnet --info 1> nul") == 0;  // NOLINT(concurrency-mt-unsafe)
}

#define TEMP_FILE_PATH "%temp%/xnor_dotnet_list_runtimes.txt"
bool DotnetRuntime::CheckDotnetVersion()
{
    // This function runs the 'dotnet --list-runtimes' command
    // This prints a list of all installed .NET runtimes on the current machine
    // We redirect the command output to TEMP_FILE_PATH and read it line by line
    // to find one that suits us, e.g. one whose version is more recent than the
    // DotnetMinVersionMajor and DotnetMinVersionMinor constants
    // Once this is done, we know for sure that the C# assemblies can be executed and let
    // the system choose the right version
    
    std::system("dotnet --list-runtimes 1> " TEMP_FILE_PATH);  // NOLINT(concurrency-mt-unsafe)

    // Expand the %temp% environment variable
    // This is done automatically in terminal commands but we need to do it manually for our strings
    char_t* buffer = static_cast<char_t*>(_malloca(MAX_PATH));
    ExpandEnvironmentStringsA(TEMP_FILE_PATH, buffer, MAX_PATH);
    
    File file(buffer);
    
    file.Load();

    constexpr const char_t* dotnetCoreName = "Microsoft.NETCore.App";
    const size_t dotnetCoreNameLength = strlen(dotnetCoreName);

    const char_t* const data = file.GetData();
    std::stringstream stream(data);
    std::string line;
    bool_t foundValidDotnet = false;
    while (!stream.eof())
    {
        std::getline(stream, line);

        if (!line.starts_with(dotnetCoreName))
            continue;

        std::string sub = line.substr(dotnetCoreNameLength + 1);
        int32_t major, minor;
        (void) sscanf_s(sub.c_str(), "%d.%d", &major, &minor);
        
        if (major > DotnetMinVersionMajor || (major == DotnetMinVersionMajor && minor >= DotnetMinVersionMinor))
        {
            foundValidDotnet = true;
            break;
        }
    }
    
    file.Unload();
    std::filesystem::remove(buffer);

    _freea(buffer);
    
    return foundValidDotnet;
}
#undef TEMP_FILE_PATH

void DotnetRuntime::CoralMessageCallback(std::string_view message, const Coral::MessageLevel level)
{
    Logger::Log(CoralMessageLevelToXnor(level), "[.NET] {}", message);
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
