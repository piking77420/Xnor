#include "scene/component/skinned_mesh_renderer.hpp"

#include "rendering/animator.hpp"

using namespace XnorCore;

void SkinnedMeshRenderer::Begin()
{
}

void SkinnedMeshRenderer::Update()
{
    if (m_CurrentMontage)
    {
        m_CurrentMontage->Update(this);
    }

    m_Animator.Animate();
}

void SkinnedMeshRenderer::StartAnimation(const Animation* const animation)
{
    m_Animator.Start(animation);
}

void SkinnedMeshRenderer::StartBlending(const Animation* const target)
{
    m_TargetAnimator.Start(target);
    m_Animator.StartBlending(&m_TargetAnimator);
}

void SkinnedMeshRenderer::SetCrossFadeDelta(const float_t delta)
{
    m_Animator.SetCrossFadeDelta(delta);
}

void SkinnedMeshRenderer::StartMontage(AnimationMontage* const montage)
{
    m_CurrentMontage = montage;
    m_CurrentMontage->Start();
}

const List<Matrix>& SkinnedMeshRenderer::GetMatrices() const
{
    return m_Animator.GetMatrices();
}
