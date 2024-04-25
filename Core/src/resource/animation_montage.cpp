#include "resource/animation_montage.hpp"

using namespace XnorCore;

void AnimationMontage::Start()
{
    m_NotifiesTimeline.Start();
    m_AnimationTimeline.Start();
    m_Ended = false;
}

void AnimationMontage::Update()
{
    if (!m_Ended)
    {
        m_NotifiesTimeline.Update();
        m_Ended = m_AnimationTimeline.Update();
    }
}

void AnimationMontage::AddEvent(const float_t when, const FunctionT& function)
{
    m_NotifiesTimeline.AddBeginEvent(when, function);
}

void AnimationMontage::AddAnimation(const float_t when, const size_t animationId)
{
    const Animation* const anim = mesh->GetAnimation(animationId);

    if (anim == nullptr)
    {
        Logger::LogError("Animation with id {} doesn't exist", animationId);
        return;
    }

    UpdateTimelineDuration(anim->GetDuration());

    m_AnimationTimeline.SetEventDuration(when, anim->GetDuration());

    m_AnimationTimeline.AddBeginEvent(when, [this, anim]() -> void
    {
        // m_MeshRenderer->StartAnimation(anim);
    });
}

void AnimationMontage::AddAnimation(const float_t when, const Animation* animation)
{
    UpdateTimelineDuration(animation->GetDuration());

    m_AnimationTimeline.SetEventDuration(when, animation->GetDuration());

    m_AnimationTimeline.AddBeginEvent(when, [this, animation]() -> void
    {
        // m_MeshRenderer->StartAnimation(animation);
    });
}

void AnimationMontage::AddAnimationBlending(const float_t when, const size_t sourceAnimation, const size_t targetAnimation, const float_t duration)
{
    const Animation* const sourceAnim = mesh->GetAnimation(sourceAnimation);
    const Animation* const targetAnim = mesh->GetAnimation(targetAnimation);

    if (sourceAnim == nullptr)
    {
        Logger::LogError("Animation with id {} doesn't exist", sourceAnimation);
        return;
    }

    if (targetAnim == nullptr)
    {
        Logger::LogError("Animation with id {} doesn't exist", targetAnimation);
        return;
    }

    UpdateTimelineDuration(duration);

    m_AnimationTimeline.SetEventDuration(when, duration);

    m_AnimationTimeline.AddBeginEvent(when, [&]() -> void
    {
        // m_MeshRenderer->StartAnimation(sourceAnim);
        // m_MeshRenderer->StartBlending(targetAnim);
    });

    m_AnimationTimeline.AddUpdateEvent(when, [&](const float_t deltaTime) -> void
    {
        // m_MeshRenderer->SetCrossFadeDelta(deltaTime);
    });
}

bool_t AnimationMontage::HasEnded() const
{
    return m_Ended;
}

void AnimationMontage::UpdateTimelineDuration(const float_t duration)
{
    m_AnimationTimeline.SetDuration(m_AnimationTimeline.GetDuration() + duration);
    m_NotifiesTimeline.SetDuration(m_NotifiesTimeline.GetDuration() + duration);
}
