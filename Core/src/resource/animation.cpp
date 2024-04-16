#include "resource/animation.hpp"

#include "input/time.hpp"
#include "rendering/rhi_typedef.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

void Animation::Update()
{
    if (!m_Skeleton)
        return;

    m_Time += Time::GetDeltaTime();

    if (m_Time >= m_Duration)
        m_Time = 0.f;

    m_CurrentFrame = static_cast<size_t>(m_Time / m_Framerate);

    const List<Bone>& bones = m_Skeleton->GetBones();

    m_CurrentFrameMatrices.Fill(Matrix::Identity());

    const Bone* const root = bones.Find([](const Bone* b) -> bool_t { return b->parentId == -1; });
    UpdateBoneTransform(root, Matrix::Identity());
}

void Animation::BindSkeleton(const Skeleton* const skeleton)
{
    m_Skeleton = skeleton;
}

bool_t Animation::Load(const uint8_t* const, const int64_t)
{
    return false;
}

bool_t Animation::Load(const aiAnimation& loadedData)
{
    m_FrameCount = static_cast<int32_t>(loadedData.mDuration);
    m_Framerate = 1.f / loadedData.mTicksPerSecond;
    m_Duration = loadedData.mDuration * m_Framerate;

    for (uint32_t i = 0; i < loadedData.mNumChannels; i++)
    {
        const aiNodeAnim* const channel = loadedData.mChannels[i];
        std::string name = channel->mNodeName.C_Str();

        auto&& it = m_KeyFrames.emplace(name, channel->mNumPositionKeys);

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

    m_CurrentFrameMatrices.Resize(loadedData.mNumChannels);
    
    return true;
}

const List<Matrix>& Animation::GetMatrices() const
{
    return m_CurrentFrameMatrices;
}

void Animation::UpdateBoneTransform(const Bone* const bone, const Matrix& parentMatrix)
{
    const List<Bone>& bones = m_Skeleton->GetBones();

    const auto it = m_KeyFrames.find(bone->name);
    const List<KeyFrame>& keyFrames = it->second;

    const size_t frame = Utils::RemapValue(m_CurrentFrame, Vector2i(0, m_FrameCount), Vector2i(0, static_cast<int32_t>(keyFrames.GetSize())));

    const Matrix localAnim = Matrix::Trs(keyFrames[frame].translation, keyFrames[frame].rotation, Vector3(1.f));
    const Matrix globalTransform = parentMatrix * localAnim;

    m_CurrentFrameMatrices[bone->id] = globalTransform * bone->global;
    
    for (size_t i = 0; i < bone->children.GetSize(); i++)
        UpdateBoneTransform(&bones[bone->children[i]], globalTransform);
}
