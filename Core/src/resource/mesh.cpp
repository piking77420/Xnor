#include "resource/mesh.hpp"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "utils/logger.hpp"

using namespace XnorCore;

Mesh::~Mesh()
{
    for (uint32_t i = 0; i < models.GetSize(); i++)
        delete models[i];

    for (uint32_t i = 0; i < m_Textures.GetSize(); i++)
        delete m_Textures[i];

    for (uint32_t i = 0; i < m_Skeletons.GetSize(); i++)
        delete m_Skeletons[i];

    for (uint32_t i = 0; i < m_Animations.GetSize(); i++)
        delete m_Animations[i];

    models.Clear();
    m_Textures.Clear();
    m_Skeletons.Clear();
    m_Animations.Clear();
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

        // model->CreateInRhi();
        models.Add(model);
    }

    /*
    for (uint32_t i = 0; i < scene->mNumTextures; i++)
    {
        break;
        Texture* const texture = new Texture(scene->mTextures[i]->mFilename.C_Str());

        const int64_t size = static_cast<int64_t>(static_cast<uint64_t>(scene->mTextures[i]->mWidth) * static_cast<uint64_t>(scene->mTextures[i]->mHeight) * sizeof(aiTexel));
        texture->Load(reinterpret_cast<const uint8_t*>(scene->mTextures[i]->pcData), size);
        texture->CreateInRhi();

        m_Textures.Add(texture);
    }
    */

    for (uint32_t i = 0; i < scene->mNumAnimations; i++)
    {
        Animation* const animation = new Animation(scene->mAnimations[i]->mName.C_Str());

        animation->Load(*scene->mAnimations[i]);

        m_Animations.Add(animation);
    }

    return true;
}
