#include "resource/mesh.hpp"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "resource/resource_manager.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

Mesh::~Mesh()
{
    models.Clear();
}

bool_t Mesh::Load(const uint8_t* buffer, const int64_t length)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFileFromMemory(buffer, length, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FixInfacingNormals | aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData);

    if (!scene)
    {
        Logger::LogError("An error occured while loading mesh: {}. Assimp error: {}", m_Name, importer.GetErrorString());
        return false;
    }

    const std::string folderPath = m_File->GetPathNoExtension() + '\\';
    FileManager::AddDirectory(folderPath);
    for (uint32_t i = 0; i < scene->mNumMeshes; i++)
    {
        const std::string meshName = scene->mMeshes[i]->mName.C_Str();
        const std::string fullName = folderPath + std::string(meshName) + std::string(".obj");
        
        Pointer<Model> model = ResourceManager::Add<Model>(fullName);

        if (!model->Load(*scene->mMeshes[i]))
        {
            ResourceManager::Unload(model);
            return false;
        }

        // model->Save();

        if (scene->mMeshes[i]->HasBones())
        {
            Pointer<Skeleton> skeleton = ResourceManager::Add<Skeleton>(std::string(scene->mAnimations[i]->mName.C_Str()) + ".skel");
            
            skeleton->Load(*scene->mMeshes[i]);

            if (i < scene->mNumAnimations)
                skeleton->Load(*scene, *scene->mAnimations[i]);

            skeleton->ReorderBones();
            skeleton->mesh = this;

            m_Skeletons.Add(skeleton);
        }   

        models.Add(model);
    }

    for (uint32_t i = 0; i < scene->mNumTextures; i++)
    {
        const std::string fileName = std::string(scene->mTextures[i]->mFilename.C_Str());
        const std::string fullName = folderPath + fileName;

        if (FileManager::Contains(fullName))
        {
            
        }
        
        const size_t pos = fileName.find_first_of('\\');
        if (pos != std::string::npos)
        {
            const std::string subFolder = folderPath + fileName.substr(0, pos);
            FileManager::AddDirectory(subFolder);
        }
        Pointer<Texture> texture = ResourceManager::Add<Texture>(fullName);

        int64_t size = 0;
        if (scene->mTextures[i]->mHeight == 0)
            size = scene->mTextures[i]->mWidth;
        else
            size = static_cast<int64_t>(static_cast<uint64_t>(scene->mTextures[i]->mWidth) * static_cast<uint64_t>(scene->mTextures[i]->mHeight) * sizeof(aiTexel));

        texture->Load(reinterpret_cast<const uint8_t*>(scene->mTextures[i]->pcData), size);
        texture->SetIsEmbedded();
        texture->Save();
    }

    for (uint32_t i = 0; i < scene->mNumAnimations; i++)
    {
        Pointer<Animation> animation = ResourceManager::Add<Animation>(std::string(scene->mAnimations[i]->mName.C_Str()) + ".anim");
        
        animation->Load(*scene->mAnimations[i]);
        if (m_Skeletons.GetSize() != 0)
            animation->BindSkeleton(m_Skeletons[0]);

        m_Animations.Add(animation);
    }

    /*
    if (textures.GetSize() >= 1)
    {
        aiString textureName;
        scene->mMaterials[0]->GetTexture(aiTextureType_DIFFUSE, 0, &textureName);
        
        material.albedoTexture = Pointer<Texture>::New(*textures[0]);
    }*/
    
    return true;
}

void Mesh::CreateInInterface()
{
    for (size_t i = 0; i < models.GetSize(); i++)
        models[i]->CreateInInterface();
    
    m_LoadedInInterface = true;
}

Pointer<Animation> Mesh::GetAnimation(const size_t id)
{
    if (id >= m_Animations.GetSize())
        return nullptr;
    
    return m_Animations[id];
}
