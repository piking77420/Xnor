#include "csharp/dotnet_runtime.hpp"

#include <cstdio>

#include "file/file.hpp"

#include "application.hpp"
#include "reflection/dotnet_reflection.hpp"
#include "utils/formatter.hpp"
#include "utils/message_box.hpp"

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
        MessageBox::Show(errorMessage, "Fatal Error", MessageBox::Type::Ok, MessageBox::Icon::Error);
        throw std::runtime_error(".NET is not installed on this machine");
    }

    if (!CheckDotnetVersion())
    {
        const std::string errorMessage = std::format("Invalid .NET version. XNOR Engine needs .NET {}.0", DotnetVersion);
        Logger::LogFatal(errorMessage);
        MessageBox::Show(errorMessage, "Fatal Error", MessageBox::Type::Ok, MessageBox::Icon::Error);
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

    Utils::TerminalCommand("dotnet build " + absolute(gameProjectDirectory).string());  // NOLINT(concurrency-mt-unsafe)

    return true;
}

bool_t DotnetRuntime::GetInitialized()
{
    return m_Initialized;
}

bool DotnetRuntime::CheckDotnetInstalled()
{
    // Check if the dotnet command returns a non-zero exit code
    return Utils::TerminalCommand("dotnet --info 1> nul", false) == 0;  // NOLINT(concurrency-mt-unsafe)
}

constexpr const char_t* const TempFile = "xnor_dotnet_list_runtimes.txt";
bool DotnetRuntime::CheckDotnetVersion()
{
    // This function runs the 'dotnet --list-runtimes' command
    // This prints a list of all installed .NET runtimes on the current machine
    // We redirect the command output to TEMP_FILE_PATH and read it line by line
    // to find one that suits us, e.g. one whose version is equal to the DotnetVersion constant
    // Once this is done, we know for sure that the C# assemblies can be executed and let
    // the system choose the right version

    std::filesystem::path tempPath = std::filesystem::temp_directory_path() / TempFile;
    
    Utils::TerminalCommand("dotnet --list-runtimes 1> \"" + tempPath.string() + '"', false);  // NOLINT(concurrency-mt-unsafe)
    
    File file(tempPath.string());
    
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
        int32_t major = 0, minor = 0;
        (void) sscanf_s(sub.c_str(), "%d.%d", &major, &minor);
        
        if (major == DotnetVersion)
        {
            foundValidDotnet = true;
            break;
        }
    }
    
    file.Unload();
    std::filesystem::remove(tempPath);
    
    return foundValidDotnet;
}

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
