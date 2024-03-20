#include "resource/resource_manager.hpp"

#include <array>
#include <execution>

#include "file/file_manager.hpp"
#include "resource/model.hpp"
#include "resource/shader.hpp"
#include "resource/texture.hpp"

using namespace XnorCore;

void ResourceManager::LoadAll()
{
    Logger::LogInfo("Loading all resources from FileManager");

    auto&& start = std::chrono::system_clock::now();

    std::vector<Pointer<File>> files;
    FileManager::FindAll<File>([](Pointer<File> file) { return file->GetResource() == nullptr; }, &files);

    const size_t oldResourceCount = m_Resources.size();

    std::for_each(
        std::execution::par,
        files.begin(),
        files.end(),
        [](auto&& file) -> void
        {
            if (std::ranges::find(Texture::FileExtensions, file->GetExtension()) != Texture::FileExtensions.end())
            {
                Load<Texture>(file, false);
            }
            else if (std::ranges::find(Model::FileExtensions, file->GetExtension()) != Model::FileExtensions.end())
            {
                Load<Model>(file, false);
            }
        }
    );

    for (auto&& file : files)
    {
        if (std::ranges::find(Shader::VertexFileExtensions, file->GetExtension()) != Shader::VertexFileExtensions.end() ||
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
        else
        {
            if (Contains(file))
                Get(file)->CreateInRhi();
        }
    }

    Logger::LogInfo(
        "Successfully loaded {} files in {} resources. Took {}",
        files.size(),
        m_Resources.size() - oldResourceCount,
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start)
    );
}

void ResourceManager::LoadGuidMap()
{
    const Pointer<File> guidMap = FileManager::Get(GuidMapFilePath);
    const char_t* const dataRaw = guidMap.Get()->GetData();
    const size_t dataSize = guidMap.Get()->GetSize();

    std::string data = std::string(dataRaw);

    size_t position = 0; // TODO Fix loop bavure
    while (position < dataSize)
    {
        const size_t guidPos = data.find_first_of(';');

        const std::string resourceName = data.substr(0, guidPos);
        const Guid guid = Guid::FromString(&data[guidPos + 1]);

        const size_t backslashPos = data.find_first_of('\n');
        position += backslashPos;

        data = data.substr(backslashPos + 1);

        auto&& it = m_Resources.find(resourceName);

        if (it == m_Resources.end())
        {
            //Logger::LogInfo("Resource in the guid map wasn't found : {}", resourceName);
        }
        else
        {
            it->second->SetGuid(guid);
            m_GuidMap.emplace(guid, it->second);
        }

        //Logger::LogInfo("{} ; {}", resourceName, static_cast<std::string>(guid));
    }
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
    
    if (resource->second->IsLoadedInRhi())
        resource->second->DestroyInRhi();

    if (resource->second->IsLoaded())
        resource->second->Unload();

    m_Resources.erase(resource);
}

void ResourceManager::UnloadAll()
{
    Logger::LogInfo("Unloading all resources ({})", m_Resources.size());

    auto&& start = std::chrono::system_clock::now();
    
    for (auto& resource : m_Resources)
    {
        Logger::LogDebug("Unloading resource {}", resource.first);
        
        if (resource.second->IsLoadedInRhi())
            resource.second->DestroyInRhi();
        
        if (resource.second->IsLoaded())
            resource.second->Unload();
    }
    // Smart pointers are deleted automatically, we only need to clear the container
    m_Resources.clear();
    
    Logger::LogInfo("ResourceManager unload successful. Took {}", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start));
}
