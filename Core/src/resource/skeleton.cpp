#include "resource/skeleton.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "utils/logger.hpp"

using namespace XnorCore;

bool_t Skeleton::Load(const uint8_t* const buffer, const int64_t length)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFileFromMemory(buffer, length, aiProcess_Triangulate | aiProcess_PopulateArmatureData);

    if (!scene)
    {
        Logger::LogError("An error occured while loading model: {}. Assimp error: {}", m_Name, importer.GetErrorString());
        return false;
    }

    if (!scene->hasSkeletons())
    {
        Logger::LogError("Invalid skeleton format, should contain a skeleton: {}", m_Name);
        return false;
    }

    if (scene->mNumSkeletons > 1)
    {
        Logger::LogError("Invalid skeleton format, should only contain a single skeleton: {}", m_Name);
        return false;
    }

    return Load(*scene->mSkeletons[0]);
}

bool_t Skeleton::Load(const aiSkeleton& loadedData)
{
    m_Bones.Resize(loadedData.mNumBones);
    
    for (size_t i = 0; i < loadedData.mNumBones; i++)
    {
        const aiSkeletonBone* const bone = loadedData.mBones[i];

        const Matrix* const local = reinterpret_cast<const Matrix*>(&bone->mLocalMatrix);
        const Matrix* const globalInv = reinterpret_cast<const Matrix*>(&bone->mOffsetMatrix);

        m_Bones[i].Create(*local, *globalInv, bone->mParent);
    }

    ComputeGlobalMatrices();
    
    return true;
}

void Skeleton::ComputeGlobalMatrices()
{
    for (size_t i = 0; i < m_Bones.GetSize(); i++)
    {
        Bone& bone = m_Bones[i];
        const int32_t parentId = bone.parentId;

        if (parentId != -1)
        {
            bone.global = m_Bones[parentId].global * bone.local;
        }
    }
}
