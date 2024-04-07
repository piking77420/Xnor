#include "csharp/dotnet_runtime.hpp"

#include <cstdio>

#include "file/file.hpp"

#include "application.hpp"
#include "reflection/dotnet_reflection.hpp"

// We need to include Window.h here but it must be the last include as it breaks everything otherwise
#undef APIENTRY
#define XMLDocument XMLDocument_dont_care
#include <windows.h>
#undef XMLDocument

using namespace XnorCore;

constexpr const char_t* AlcName = "XNOR Coral AssemblyLoadContext";

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
        constexpr const char_t* const errorMessage = ".NET is not installed on this machine";
        Logger::LogFatal(errorMessage);
        MessageBoxA(nullptr, errorMessage, "Fatal Error", MB_OK | MB_ICONSTOP);
        throw std::runtime_error(".NET is not installed on this machine");
    }

    if (!CheckDotnetVersion())
    {
        const std::string errorMessage = std::format("Invalid .NET version. XNOR Engine needs .NET {}.0", DotnetVersionMajor);
        Logger::LogFatal(errorMessage);
        MessageBoxA(nullptr, errorMessage.c_str(), "Fatal Error", MB_OK | MB_ICONSTOP);
        throw std::runtime_error("Invalid .NET version");
    }

    m_AssembliesPath = Application::executablePath.parent_path() / AssembliesDirectory;
    m_Settings.CoralDirectory = m_AssembliesPath.string();
    
    if (!m_Runtime.Initialize(m_Settings))
    {
        Logger::LogError("An unknown error occured while initializing .NET runtime");
        return false;
    }

    m_Alc = m_Runtime.CreateAssemblyLoadContext(AlcName);

    if (!LoadAssembly("CoreCSharp"))
    {
        Logger::LogError("An unknown error occured while loading XNOR .NET library");
        return false;
    }

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
    const std::filesystem::path&& filepath = m_AssembliesPath / (name + ".dll");
    
    Logger::LogInfo("Loading .NET assembly {}", filepath.filename());

    const std::string&& str = filepath.string();
    
    DotnetAssembly* const assembly = new DotnetAssembly(str);
    if (assembly->Load(m_Alc))
    {
        //assembly->ProcessTypes();
        m_LoadedAssemblies.push_back(assembly);
        return true;
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
    {
        if (LoadAssembly(assembly))
            continue;

        Logger::LogWarning("Couldn't reload assembly {}", assembly);
    }
}

bool_t DotnetRuntime::BuildGameProject()
{
    constexpr const char_t* const gameProjectLocation = "Game";
    
    const std::filesystem::path gameProjectDirectory = gameProjectLocation;

    if (!exists(gameProjectDirectory))
        return false;

    if (!is_directory(gameProjectDirectory))
        return false;

    if (!exists(gameProjectDirectory / "Game.csproj"))
        return false;

    std::system(("start dotnet build " + absolute(gameProjectDirectory).string()).c_str());  // NOLINT(concurrency-mt-unsafe)

    return true;
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
    const size_t dotnetCoreNameLength = std::strlen(dotnetCoreName);

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
        
        if (major == DotnetVersionMajor)
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
    Logger::LogError("Unhandled .NET exception: {}", message);
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
