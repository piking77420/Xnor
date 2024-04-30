#include "csharp/dotnet_runtime.hpp"

#include <cstdio>

#include "file/file.hpp"

#include "application.hpp"
#include "csharp/dotnet_constants.hpp"
#include "reflection/dotnet_reflection.hpp"
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

    m_AssembliesPath = Application::executablePath.parent_path() / Dotnet::AssembliesDirectory;
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
        assembly->ProcessTypes();
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

bool_t DotnetRuntime::BuildGameProject(const bool_t asynchronous)
{
    const std::filesystem::path gameProjectDirectory = Dotnet::GameProjectLocation;

    if (!exists(gameProjectDirectory))
        return false;

    if (!is_directory(gameProjectDirectory))
        return false;

    if (!exists(gameProjectDirectory / "Game.csproj"))
        return false;

    Logger::LogInfo("Building Game project");

    static constexpr const char_t* const TempFile = "xnor_dotnet_build.txt";
    const std::filesystem::path tempPath = std::filesystem::temp_directory_path() / TempFile;

    using namespace std::string_literals;

    const int32_t commandResult = Utils::TerminalCommand("dotnet build "s + Dotnet::GameProjectBuildOptions + " \"" + absolute(gameProjectDirectory).string() + "\" 1> \"" + tempPath.string() + '"', asynchronous);

    // In case a warning/error occured, read the output file to understand what happened
    std::ifstream file(tempPath);
    std::string line;

    // Dotnet outputs look like this:
    // <MSBuild version>
    //   <Projects being restored>
    //   <Projects being built>
    //
    // Build <succeeded/FAILED>
    //
    // <Warning list>
    // <Error list>
    //     <Total warning count>
    //     <Total error count>
    //
    // <Time elapsed>
    
    while (!line.starts_with("Build succeeded") && !line.starts_with("Build FAILED"))
        std::getline(file, line);

    // Here we are right after the 'Build <succeeded/FAILED>' line

    std::getline(file, line);

    // This is the first potential warning/error line
    std::getline(file, line);

    std::vector<std::string> diagnostics;
    while (!line.starts_with("    "))
    {
        diagnostics.push_back(line);
        std::getline(file, line);
    }

    if (!diagnostics.empty())
    {
        for (auto&& str : diagnostics)
            Logger::LogWarning("[.NET BUILD] {}", str);
    }

    file.close();

    std::filesystem::remove(tempPath);

    if (commandResult == 0)
    {
        Logger::LogInfo("Build succeeded");
        return true;
    }
    
    Logger::LogError("Build failed");
    return false;
}

void DotnetRuntime::BuildAndReloadProject()
{
    m_ReloadingProjectAsync = true;

    if (m_ProjectReloadingThread.joinable())
        m_ProjectReloadingThread.join();

    m_ProjectReloadingThread = std::thread(
        []
        {
            if (BuildGameProject(false))
                ReloadAllAssemblies();
            m_ReloadingProjectAsync = false;
        }
    );
}

bool_t DotnetRuntime::GetInitialized()
{
    return m_Initialized;
}

bool_t DotnetRuntime::IsReloadingProject()
{
    return m_ReloadingProjectAsync;
}

bool DotnetRuntime::CheckDotnetInstalled()
{
    // Check if the dotnet command returns a non-zero exit code
    return Utils::TerminalCommand("dotnet --info 1> nul", false) == 0;
}

bool DotnetRuntime::CheckDotnetVersion()
{
    // This function runs the 'dotnet --list-runtimes' command
    // This prints a list of all installed .NET runtimes on the current machine
    // We redirect the command output to TEMP_FILE_PATH and read it line by line
    // to find one that suits us, e.g. one whose version is equal to the DotnetVersion constant
    // Once this is done, we know for sure that the C# assemblies can be executed and let
    // the system choose the right version

    static constexpr const char_t* const TempFile = "xnor_dotnet_list_runtimes.txt";
    const std::filesystem::path tempPath = std::filesystem::temp_directory_path() / TempFile;
    
    Utils::TerminalCommand("dotnet --list-runtimes 1> \"" + tempPath.string() + '"', false);
    
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
