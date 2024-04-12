#include "resource/animation.hpp"

using namespace XnorCore;

bool_t Animation::Load(const uint8_t* const buffer, const int64_t length)
{
    return Resource::Load(buffer, length);
}

bool_t Animation::Load(const aiAnimation& loadedData)
{
    m_Duration = loadedData.mDuration * loadedData.mTicksPerSecond;
    m_Framerate = loadedData.mTicksPerSecond;

    for (uint32_t i = 0; i < loadedData.mNumChannels; i++)
    {
        const aiNodeAnim* const channel = loadedData.mChannels[i];

        const size_t keyCount = channel->mNumPositionKeys;

        m_KeyFrames.emplace_back(keyCount);

        for (size_t j = 0; j < keyCount; j++)
        {
            const Vector3 translation = Vector3(&channel->mPositionKeys[j].mValue.x);
            const Quaternion rotation = Quaternion(&channel->mRotationKeys[j].mValue.x);
            const Vector3 scaling = Vector3(&channel->mScalingKeys[j].mValue.x);

            const KeyFrame keyFrame =
            {
                .translation = translation,
                .rotation = rotation,
                .scaling = scaling
            };
            
            m_KeyFrames[i][j] = keyFrame;
        }
    }

    return true;
}
