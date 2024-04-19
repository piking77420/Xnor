#include "rendering/animator.hpp"

#include "input/time.hpp"
#include "utils/utils.hpp"
#include "resource/animation.hpp"

using namespace XnorCore;

Animator::Animator(const Animation* animation)
    : m_Animation(animation), m_FrameCount(animation->GetFrameCount())
{
}

void Animator::Start(const Animation* animation)
{
    m_Animation = animation;
    m_FrameCount = animation->GetFrameCount();
}

void Animator::StartBlending(Animator* const target)
{
    m_BlendTarget = target;
}

void Animator::Animate()
{
    if (!m_Animation)
        return;
    
    const Skeleton* const skeleton = m_Animation->GetSkeleton();
    
    if (!skeleton)
        return;

    UpdateTime();

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
    float_t t = std::fmodf(m_Time, m_Animation->GetFrameDuration()) / m_Animation->GetFrameDuration();
    if (m_PlaySpeed < 0)
        t = 1 - t;

    const List<Bone>& bones = skeleton->GetBones();
    List<Matrix> currentMatrices(bones.GetSize());

    if (m_BlendTarget)
    {
        m_BlendTarget->m_PlaySpeed = m_BlendTarget->m_Animation->GetDuration() / m_Animation->GetDuration() * m_PlaySpeed;
        m_BlendTarget->Animate();
    }

    for (size_t i = 0; i < bones.GetSize(); i++)
    {
        const Bone& bone = bones[i];
        const List<Animation::KeyFrame>& keyFrames = m_Animation->GetBoneKeyFrame(bone);

        const size_t frame = Utils::RemapValue(m_CurrentFrame, Vector2i(0, static_cast<int32_t>(m_FrameCount)), Vector2i(0, static_cast<int32_t>(keyFrames.GetSize())));

        m_Positions[i] = Vector3::Lerp(keyFrames[frame].translation, keyFrames[nextFrame].translation, t);
        m_Rotations[i] = Quaternion::Slerp(keyFrames[frame].rotation, keyFrames[nextFrame].rotation, t);

        if (m_BlendTarget)
        {
            m_Positions[i] = Vector3::Lerp(m_Positions[i], m_BlendTarget->m_Positions[i], m_CrossFadeT);
            m_Rotations[i] = Quaternion::Slerp(m_Rotations[i], m_BlendTarget->m_Rotations[i], m_CrossFadeT);
        }

        const Matrix localAnim = Matrix::Trs(m_Positions[i], m_Rotations[i], Vector3(1.f));

        if (bone.parentId != -1)
        {
            // The bone has a parent, so apply the parent global transform to it
            currentMatrices[bone.id] = currentMatrices[bones[bone.parentId].id] * localAnim;
        }
        else
        {
            // The bone has no parent, so its global transform is the same as its local
            currentMatrices[bone.id] = localAnim;
        }
	    
        // Apply the inverse to the global transform to remove the bind pose transform
        m_FinalMatrices[bone.id] = currentMatrices[bone.id] * bone.global;
    }
}

void Animator::SetCrossFadeDelta(const float_t delta)
{
    m_CrossFadeT = delta;
}

const List<Matrix>& Animator::GetMatrices() const
{
    return m_FinalMatrices;
}

void Animator::UpdateTime()
{
    m_Time += Time::GetDeltaTime() * m_PlaySpeed;
    
    if (m_Time >= m_Animation->GetDuration())
    {
        m_Time = 0.f;
        m_IsFinished = true;
    }

    if (m_PlaySpeed < 0.f && m_Time <= 0)
    {
        m_Time = m_Animation->GetDuration();
        m_IsFinished = true;
    }

    m_CurrentFrame = std::min(static_cast<size_t>(m_Time / m_Animation->GetFrameDuration()), m_FrameCount - 1);
}
