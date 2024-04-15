#include "resource/animation.hpp"

#include "input/time.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

void Animation::Update()
{
    m_Time += Time::GetDeltaTime();

    if (m_Time >= m_Duration)
        m_Time = 0.f;

    m_CurrentFrame = static_cast<size_t>(m_Time / m_Framerate);

    CalculateBoneTransform(&m_RootNode, Matrix::Identity());
    return;
    const List<Bone>& bones = m_Skeleton->GetBones();

    m_CurrentFrameMatrices.Fill(Matrix::Identity());

    for (size_t i = 0; i < bones.GetSize(); i++)
    {
        const auto&& it = m_KeyFrames.find(bones[i].name);

        const List<KeyFrame>& keyFrames = it->second;

        const size_t frame = Utils::RemapValue(m_CurrentFrame, Vector2i(0, m_FrameCount), Vector2i(0, static_cast<int32_t>(keyFrames.GetSize())));

        const Matrix localAnim = Matrix::Trs(keyFrames[frame].translation, keyFrames[frame].rotation, Vector3(1.f));

        for (size_t j = 0; j < bones[i].children.GetSize(); j++)
        {
            m_CurrentFrameMatrices[bones[i].children[j]] *= localAnim;
        }

        m_CurrentFrameMatrices[i] *= localAnim;
    }
}

bool_t Animation::Load(const uint8_t* const, const int64_t)
{
    return false;
}

bool_t Animation::Load(const aiScene& scene, const aiAnimation& loadedData, Skeleton* const skeleton)
{
    m_FrameCount = static_cast<int32_t>(loadedData.mDuration);
    m_Framerate = 1.f / loadedData.mTicksPerSecond;
    m_Duration = loadedData.mDuration * m_Framerate;

    m_Skeleton = skeleton;

    ReadHierarchyData(m_RootNode, scene.mRootNode->mChildren[1]);

    for (uint32_t i = 0; i < loadedData.mNumChannels; i++)
    {
        const aiNodeAnim* const channel = loadedData.mChannels[i];

        auto&& it = m_KeyFrames.emplace(channel->mNodeName.C_Str(), channel->mNumPositionKeys);

        for (uint32_t j = 0; j < channel->mNumPositionKeys; j++)
        {
            const Vector3 translation = Vector3(&channel->mPositionKeys[j].mValue.x);
            const Quaternion rotation = Quaternion(Vector3(&channel->mRotationKeys[j].mValue.x), channel->mRotationKeys[j].mValue.w);
            const Vector3 scaling = Vector3(&channel->mScalingKeys[j].mValue.x);

            const KeyFrame keyFrame =
            {
                .translation = translation,
                .rotation = rotation,
                .scaling = scaling,
                .time = static_cast<float_t>(channel->mPositionKeys[j].mTime)
            };

            it.first->second[j] = keyFrame;
        }
    }

    const List<Bone>& bones = m_Skeleton->GetBones();
    if (bones.GetSize() != loadedData.mNumChannels)
        LoadMissingBones(loadedData);

    m_CurrentFrameMatrices.Resize(loadedData.mNumChannels);
    
    return true;
}

const List<Matrix>& Animation::GetMatrices() const
{
    return m_CurrentFrameMatrices;
}

void Animation::LoadMissingBones(const aiAnimation& loadedData)
{
    const uint32_t size = loadedData.mNumChannels;

    List<Bone>& bones = m_Skeleton->GetBones();

    for (uint32_t i = 0; i < size; i++)
    {
        const aiNodeAnim* const channel = loadedData.mChannels[i];
        std::string boneName = channel->mNodeName.data;

        if (bones.Exists([&boneName](const Bone* b) -> bool_t { return b->name == boneName; }))
            continue;

        Bone bone;
        bone.name = boneName;
        bone.local = Matrix::Identity();
        bone.global = Matrix::Identity();
        bone.globalInverse = Matrix::Identity();
        bone.id = static_cast<int32_t>(bones.GetSize());

        bones.Add(bone);

        auto&& it = m_KeyFrames.emplace(boneName, channel->mNumPositionKeys);

        for (uint32_t j = 0; j < channel->mNumPositionKeys; j++)
        {
            const Vector3 translation = Vector3(&channel->mPositionKeys[j].mValue.x);
            const Quaternion rotation = Quaternion(Vector3(&channel->mRotationKeys[j].mValue.x), channel->mRotationKeys[j].mValue.w);
            const Vector3 scaling = Vector3(&channel->mScalingKeys[j].mValue.x);

            const KeyFrame keyFrame =
            {
                .translation = translation,
                .rotation = rotation,
                .scaling = scaling,
                .time = static_cast<float_t>(channel->mPositionKeys[j].mTime)
            };

            it.first->second[j] = keyFrame;
        }
    }
}

void Animation::ReadHierarchyData(AssimpNodeData& dest, const aiNode* src)
{
    const Matrix* const local = reinterpret_cast<const Matrix*>(&src->mTransformation);
    dest.name = src->mName.data;
    dest.transformation = local->Transposed();
    dest.children.Resize(src->mNumChildren);

    for (uint32_t i = 0; i < src->mNumChildren; i++)
    {
        AssimpNodeData newData;
        ReadHierarchyData(newData, src->mChildren[i]);
        dest.children[i] = newData;
    }
}

void Animation::CalculateBoneTransform(const AssimpNodeData* node, const Matrix& parentTransform)
{
    const List<Bone> bones = m_Skeleton->GetBones();

    const Bone* const bone = bones.Find([&node](const Bone* const b) -> bool_t { return node->name == b->name; });

    const auto&& it = m_KeyFrames.find(bone->name);

    const List<KeyFrame>& keyFrames = it->second;

    const size_t frame = Utils::RemapValue(m_CurrentFrame, Vector2i(0, m_FrameCount), Vector2i(0, static_cast<int32_t>(keyFrames.GetSize())));

    const Matrix localAnim = Matrix::Trs(keyFrames[frame].translation, keyFrames[frame].rotation, Vector3(1.f));

    const Matrix globalTransformation = parentTransform * localAnim;

    m_CurrentFrameMatrices[bone->id] = globalTransformation * bone->global;

    for (size_t i = 0; i < node->children.GetSize(); i++)
        CalculateBoneTransform(&node->children[i], globalTransformation);
}
