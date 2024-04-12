#include "resource/animation.hpp"

#include "input/time.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

void Animation::Update()
{
    m_Time += Time::GetDeltaTime();

    if (m_Time >= m_Duration)
        m_Time = 0.f;

    const size_t currentFrame = static_cast<size_t>(m_Time / m_Framerate);
    
    const List<Bone>& bones = m_Skeleton->GetBones();

    m_CurrentFrameMatrices.Fill(Matrix::Identity());

    for (size_t i = 0; i < bones.GetSize(); i++)
    {
        const auto&& it = m_KeyFrames.find(bones[i].name);

        const List<KeyFrame>& keyFrames = it->second;

        const size_t frame = Utils::RemapValue(currentFrame, Vector2i(0, m_FrameCount), Vector2i(0, static_cast<int32_t>(keyFrames.GetSize())));

        const Matrix localAnim = Matrix::Trs(keyFrames[frame].translation, keyFrames[frame].rotation, Vector3(1.f));

        for (size_t j = 0; j < bones[i].children.GetSize(); j++)
        {
            m_CurrentFrameMatrices[bones[i].children[j]] *= localAnim;
        }

        m_CurrentFrameMatrices[i] *= localAnim;
    }
}

void Animation::BindSkeleton(Skeleton* skeleton)
{
    m_Skeleton = skeleton;
}

bool_t Animation::Load(const uint8_t* const buffer, const int64_t length)
{
    return Resource::Load(buffer, length);
}

bool_t Animation::Load(const aiAnimation& loadedData)
{
    m_FrameCount = static_cast<int32_t>(loadedData.mDuration);
    m_Framerate = 1.f / loadedData.mTicksPerSecond;
    m_Duration = loadedData.mDuration * m_Framerate;

    for (uint32_t i = 0; i < loadedData.mNumChannels; i++)
    {
        const aiNodeAnim* const channel = loadedData.mChannels[i];

        auto&& it = m_KeyFrames.emplace(channel->mNodeName.C_Str(), channel->mNumPositionKeys);

        for (uint32_t j = 0; j < channel->mNumPositionKeys; j++)
        {
            const Vector3 translation = Vector3(&channel->mPositionKeys[j].mValue.x);
            const Quaternion rotation = Quaternion(&channel->mRotationKeys[j].mValue.w);
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
