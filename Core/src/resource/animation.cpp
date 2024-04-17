#include "resource/animation.hpp"

#include "input/time.hpp"
#include "rendering/rhi_typedef.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

void Animation::Update()
{
    if (!m_Skeleton)
        return;

    m_Time += Time::GetDeltaTime() * m_PlaySpeed;
    m_Time = std::fmodf(m_Time, m_Duration);

    if (m_PlaySpeed < 0.f && m_Time <= 0)
    {
        m_Time = m_Duration;
    }

    m_CurrentFrame = std::min(static_cast<size_t>(m_Time / m_FrameDuration), m_FrameCount - 1);

    size_t nextFrame = m_CurrentFrame;
    if (m_PlaySpeed < 0)
    {
        if (m_CurrentFrame == 0)
            nextFrame = m_FrameCount - 1;
        else
            nextFrame = (m_CurrentFrame - 1) % m_FrameCount;
    }
    else
    {
        nextFrame = (m_CurrentFrame + 1) % m_FrameCount;
    }
    float_t t = std::fmodf(m_Time, m_FrameDuration) / m_FrameDuration;
    if (m_PlaySpeed < 0)
        t = 1 - t;

    const List<Bone>& bones = m_Skeleton->GetBones();

    for (size_t i = 0; i < bones.GetSize(); i++)
    {
        const Bone& bone = bones[i];
        const auto&& it = m_KeyFrames.find(bone.name);

        const List<KeyFrame>& keyFrames = it->second;

        const size_t frame = Utils::RemapValue(m_CurrentFrame, Vector2i(0, static_cast<int32_t>(m_FrameCount)), Vector2i(0, static_cast<int32_t>(keyFrames.GetSize())));

        const Vector3 position = Vector3::Lerp(keyFrames[frame].translation, keyFrames[nextFrame].translation, t);
        const Quaternion rotation = Quaternion::Slerp(keyFrames[frame].rotation, keyFrames[nextFrame].rotation, t);
        const Matrix localAnim = Matrix::Trs(position, rotation, Vector3(1.f));

        if (bone.parentId != -1)
        {
            // The bone has a parent, so apply the parent global transform to it
            m_CurrentFrameMatrices[bone.id] = m_CurrentFrameMatrices[bones[bone.parentId].id] * localAnim;
        }
        else
        {
            // The bone has no parent, so its local transform is the same as its global
            m_CurrentFrameMatrices[bone.id] = localAnim;
        }
		
        // Apply the inverse to the global transform to remove the bind pose transform
        m_FinalMatrices[bone.id] = m_CurrentFrameMatrices[bone.id] * bone.global;
    }
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
    m_FrameCount = static_cast<size_t>(loadedData.mDuration);
    m_Framerate = static_cast<float_t>(loadedData.mTicksPerSecond);
    m_FrameDuration = 1.f / m_Framerate;
    m_Duration = static_cast<float_t>(loadedData.mDuration / loadedData.mTicksPerSecond);

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
    m_FinalMatrices.Resize(loadedData.mNumChannels);
    m_FinalMatrices.Fill(Matrix::Identity());

    return true;
}

const List<Matrix>& Animation::GetMatrices() const
{
    return m_FinalMatrices;
}
