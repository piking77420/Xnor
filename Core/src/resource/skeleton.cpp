#include "resource/skeleton.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "assimp/cimport.h"
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

    if (scene->mNumMeshes >= 1 && scene->mMeshes[0]->HasBones())
    {
        return Load(*scene->mMeshes[0]);
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

        m_Bones[i].Create(*local, *globalInv);
        m_Bones[i].parentId = bone->mParent;
    }

    return true;
}

inline bool_t Skeleton::Load(const aiMesh& loadedData)
{
    const size_t numBones = loadedData.mNumBones;
    
    m_Bones.Resize(numBones);

    List<const aiNode*> boneInfo(numBones);

    for (size_t i = 0; i < numBones; i++)
    {
        const aiBone* const bone = loadedData.mBones[i];

        const Matrix* const local = reinterpret_cast<const Matrix*>(&bone->mOffsetMatrix);
        const Matrix* const globalInv = reinterpret_cast<const Matrix*>(&bone->mOffsetMatrix);

        Bone* const b = &m_Bones[i];
        b->name = std::string(bone->mName.data);
        b->Create(*local, *globalInv);
        b->id = static_cast<int32_t>(i);

        boneInfo[i] = bone->mNode;
    }

    for (size_t i = 0; i < numBones; i++)
    {
        const aiBone* const bone = loadedData.mBones[i];

        for (int32_t j = 0; j < static_cast<int32_t>(numBones); j++)
        {
            if (bone->mNode->mParent == boneInfo[j])
            {
                m_Bones[i].parentId = j;
                m_Bones[j].children.Add(static_cast<int32_t>(i));
                break;
            }
        }
    }

    // PrintBones();
    
    return true;
}

bool_t Skeleton::Load(const aiScene& scene, const aiAnimation& loadedData)
{
    const uint32_t size = loadedData.mNumChannels;

    for (uint32_t i = 0; i < size; i++)
    {
        const aiNodeAnim* const channel = loadedData.mChannels[i];
        std::string boneName = channel->mNodeName.data;

        if (m_Bones.Exists([&boneName](const Bone* b) -> bool_t { return b->name == boneName; }))
            continue;

        const aiNode* const node = scene.mRootNode->FindNode(channel->mNodeName);
        std::string parentName = node->mParent->mName.C_Str();
        const size_t parentPos = m_Bones.FindPosition([&parentName](const Bone* b) -> bool_t { return b->name == parentName; });

        Bone bone;
        bone.name = boneName;
        bone.local = Matrix::Identity();
        bone.global = Matrix(0.f);
        bone.globalInverse = Matrix::Identity();
        bone.id = static_cast<int32_t>(m_Bones.GetSize());

        for (uint32_t j = 0; j < node->mNumChildren; j++)
        {
            const aiNode* const childNode = node->mChildren[j];
            std::string childName = childNode->mName.C_Str();
            const size_t childPos = m_Bones.FindPosition([&childName](const Bone* b) -> bool_t { return b->name == childName; });

            if (childPos != std::numeric_limits<size_t>::max())
            {
                bone.children.Add(static_cast<int32_t>(childPos));
                m_Bones[childPos].parentId = bone.id;
            }
        }

        if (parentPos != std::numeric_limits<size_t>::max())
        {
            bone.parentId = static_cast<int32_t>(parentPos);
            m_Bones[bone.parentId].children.Add(bone.id);
        }
        
        m_Bones.Add(bone);
    }

    return true;
}

const List<Bone>& Skeleton::GetBones() const
{
    return m_Bones;
}

void Skeleton::PrintBones() const
{
    for (size_t i = 0; i < m_Bones.GetSize(); i++)
    {
        const Bone& bone = m_Bones[i];
        Logger::LogInfo("{} ; {} ; {}\n{}\n{}\n{}", bone.name, bone.position, Quaternion::ToEuler(bone.rotation) * Calc::Rad2Deg, bone.local, bone.global, bone.globalInverse);
    }
}
