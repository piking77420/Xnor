#include "csharp/dotnet_assembly.hpp"

#include "scene/component/script_component.hpp"
#include "utils/logger.hpp"
#include "world/world.hpp"

using namespace XnorCore;

DotnetAssembly::DotnetAssembly(std::string filepath)
    : m_Filepath(std::move(filepath))
{
}

bool_t DotnetAssembly::Load(Coral::AssemblyLoadContext& alc)
{
    m_Assembly = &alc.LoadAssembly(m_Filepath);
    
    switch (m_Assembly->GetLoadStatus())
    {
        case Coral::AssemblyLoadStatus::Success:
            if (m_Assembly->GetName() == "CoreCSharp")
                xnorCoreAssembly = this;
            return true;
        
        case Coral::AssemblyLoadStatus::FileNotFound:
            Logger::LogError("Couldn't find .NET assembly file {}", m_Filepath);
            break;
        
        case Coral::AssemblyLoadStatus::FileLoadFailure:
            Logger::LogError("Couldn't load .NET assembly file {}", m_Filepath);
            break;
        
        case Coral::AssemblyLoadStatus::InvalidFilePath:
            Logger::LogError("Invalid .NET assembly file path {}", m_Filepath);
            break;
        
        case Coral::AssemblyLoadStatus::InvalidAssembly:
            Logger::LogError("Invalid .NET assembly file {}", m_Filepath);
            break;
        
        case Coral::AssemblyLoadStatus::UnknownError:
            Logger::LogError("An unknown error occured while loading .NET assembly file {}", m_Filepath);
            break;
    }

    return false;
}

void DotnetAssembly::ProcessTypes()
{
    if (!xnorCoreAssembly)
        return;
    
    Coral::Type& scriptComponentType = xnorCoreAssembly->GetCoralAssembly()->GetType("Xnor.Core.ScriptComponent");
    
    for (auto&& type : m_Assembly->GetTypes())
    {
        if (type->IsSubclassOf(scriptComponentType))
            ProcessScriptComponent(scriptComponentType, *type);
    }
}

const Coral::ManagedAssembly* DotnetAssembly::GetCoralAssembly() const
{
    return m_Assembly;
}

const std::string& DotnetAssembly::GetFilename() const
{
    return m_Filepath;
}

void DotnetAssembly::ProcessScriptComponent(Coral::Type& scriptComponent, Coral::Type& subclass)
{
    Entity* entity = World::scene.CreateEntity(static_cast<std::string>(subclass.GetFullName()) + " Entity");
    auto&& instance = subclass.CreateInstance();
    ScriptComponent* ptr = instance.GetFieldValue<ScriptComponent*>("swigCPtr");
    entity->AddComponent(ptr);
}
