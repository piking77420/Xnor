#include "resource/resource_manager.hpp"

#include <array>

#include "file/file_manager.hpp"
#include "resource/model.hpp"
#include "resource/shader.hpp"
#include "resource/texture.hpp"

using namespace XnorCore;

void ResourceManager::LoadAll()
{
    Logger::LogDebug("Loading all resources from FileManager");

    auto&& start = std::chrono::system_clock::now();

    std::vector<Pointer<File>> files;
    FileManager::FindAll<File>(&files);

    const size_t oldResourceCount = m_Resources.size();

    for (auto&& file : files)
    {
        if (std::ranges::find(Texture::FileExtensions, file->GetExtension()) != Texture::FileExtensions.end())
        {
            Load<Texture>(file);
        }
        else if (std::ranges::find(Model::FileExtensions, file->GetExtension()) != Model::FileExtensions.end())
        {
            Load<Model>(file);
        }
        else if (std::ranges::find(Shader::VertexFileExtensions, file->GetExtension()) != Shader::VertexFileExtensions.end() ||
            std::ranges::find(Shader::FragmentFileExtensions, file->GetExtension()) != Shader::FragmentFileExtensions.end() ||
            std::ranges::find(Shader::GeometryFileExtensions, file->GetExtension()) != Shader::GeometryFileExtensions.end() ||
            std::ranges::find(Shader::ComputeFileExtensions, file->GetExtension()) != Shader::ComputeFileExtensions.end())
        {
            Pointer<Shader> shader;

            // We use an underscore before the name to make sure it isn't used elsewhere
            const std::string&& filenameNoExtension = ReservedShaderPrefix + file->GetNameNoExtension();
            if (Contains(filenameNoExtension))
                shader = Get<Shader>(filenameNoExtension);
            else
                shader = Add<Shader>(filenameNoExtension);
            
            shader->Load(file);
        }
    }

    Logger::LogDebug(
        "Successfully loaded {} files in {} resources. Took {}",
        files.size(),
        m_Resources.size() - oldResourceCount,
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start)
    );
}

bool ResourceManager::Contains(const std::string& name)
{
    return m_Resources.contains(name);
}

bool ResourceManager::Contains(const Pointer<File>& file)
{
    return m_Resources.contains(file->GetPathString());
}

void ResourceManager::Rename(const std::string& name, const std::string& newName)
{
    Rename(Get(name), newName);
}

void ResourceManager::Rename(const Pointer<Resource>& resource, const std::string& newName)
{
    std::string&& oldName = resource->GetName();

    Logger::LogInfo("Renaming resource {} to {}", oldName, newName);

    // Create a new temporary strong reference of the resource to keep it alive until we insert it in the map again
    const Pointer newResource(resource, true);

    m_Resources.erase(oldName);
    // Here we also need to create a new strong reference as the last one will be deleted when going out of scope
    m_Resources[newName] = newResource.CreateStrongReference();
}

void ResourceManager::Unload(const std::string& name)
{
    Logger::LogDebug("Unloading resource {}", name);
    
    const auto&& resource = m_Resources.find(name);
    
    if (resource == m_Resources.end())
    {
        Logger::LogWarning("Attempt to unload an unknown resource: {}", name);
        return;
    }
    
    if (resource->second->GetLoadedInRhi())
        resource->second->DestroyInRhi();

    if (resource->second->GetLoaded())
        resource->second->Unload();

    m_Resources.erase(resource);
}

void ResourceManager::UnloadAll()
{
    Logger::LogDebug("Unloading all resources ({})", m_Resources.size());

    auto&& start = std::chrono::system_clock::now();
    
    for (auto& resource : m_Resources)
    {
        Logger::LogDebug("Unloading resource {}", resource.first);
        
        if (resource.second->GetLoadedInRhi())
            resource.second->DestroyInRhi();
        
        if (resource.second->GetLoaded())
            resource.second->Unload();
    }
    // Smart pointers are deleted automatically, we only need to clear the container
    m_Resources.clear();
    
    Logger::LogDebug("ResourceManager unload successful. Took {}", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start));
}
