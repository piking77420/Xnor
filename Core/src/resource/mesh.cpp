#include "resource/mesh.hpp"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "utils/logger.hpp"

using namespace XnorCore;

Mesh::~Mesh()
{
    for (uint32_t i = 0; i < models.GetSize(); i++)
        delete models[i];

    for (uint32_t i = 0; i < textures.GetSize(); i++)
        delete textures[i];

    for (uint32_t i = 0; i < m_Skeletons.GetSize(); i++)
        delete m_Skeletons[i];

    for (uint32_t i = 0; i < animations.GetSize(); i++)
        delete animations[i];

    models.Clear();
    textures.Clear();
    m_Skeletons.Clear();
    animations.Clear();
}

bool_t Mesh::Load(const uint8_t* buffer, const int64_t length)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFileFromMemory(buffer, length, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FindInvalidData | aiProcess_FixInfacingNormals | aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData);

    if (!scene)
    {
        Logger::LogError("An error occured while loading mesh: {}. Assimp error: {}", m_Name, importer.GetErrorString());
        return false;
    }

    for (uint32_t i = 0; i < scene->mNumMeshes; i++)
    {
        Model* const model = new Model(scene->mMeshes[i]->mName.C_Str());

        if (!model->Load(*scene->mMeshes[i]))
        {
            delete model;
            return false;
        }

        if (scene->mMeshes[i]->HasBones())
        {
            Skeleton* const skeleton = new Skeleton();

            skeleton->Load(*scene->mMeshes[i]);
            m_Skeletons.Add(skeleton);
        }

        models.Add(model);
    }

    for (uint32_t i = 0; i < scene->mNumTextures; i++)
    {
        Texture* const texture = new Texture(scene->mTextures[i]->mFilename.C_Str());

        int64_t size = 0;
        if (scene->mTextures[i]->mHeight == 0)
            size = scene->mTextures[i]->mWidth;
        else
            size = static_cast<int64_t>(static_cast<uint64_t>(scene->mTextures[i]->mWidth) * static_cast<uint64_t>(scene->mTextures[i]->mHeight) * sizeof(aiTexel));
    
        texture->Load(reinterpret_cast<const uint8_t*>(scene->mTextures[i]->pcData), size);
        texture->SetIsEmbedded();

        textures.Add(texture);
    }

    for (uint32_t i = 0; i < scene->mNumAnimations; i++)
    {
        Animation* const animation = new Animation(scene->mAnimations[i]->mName.C_Str());

        animation->Load(*scene, *scene->mAnimations[i], m_Skeletons[0]);

        animations.Add(animation);
    }

    if (textures.GetSize() >= 2)
    {
        material.albedoTexture = Pointer<Texture>::Create(*textures[0]);
    }

    return true;
}

void Mesh::CreateInRhi()
{
    for (uint32_t i = 0; i < models.GetSize(); i++)
        models[i]->CreateInRhi();

    for (uint32_t i = 0; i < textures.GetSize(); i++)
        textures[i]->CreateInRhi();

    for (uint32_t i = 0; i < m_Skeletons.GetSize(); i++)
        m_Skeletons[i]->CreateInRhi();

    for (uint32_t i = 0; i < animations.GetSize(); i++)
        animations[i]->CreateInRhi();
}
