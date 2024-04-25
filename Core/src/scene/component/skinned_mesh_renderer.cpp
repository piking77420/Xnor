#include "..\..\..\include\scene\component\skinned_mesh_renderer.hpp"

#include "rendering/animator.hpp"

using namespace XnorCore;

void SkinnedMeshRenderer::Begin()
{
}

void SkinnedMeshRenderer::Update()
{
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

const List<Matrix>& SkinnedMeshRenderer::GetMatrices() const
{
    return m_Animator.GetMatrices();
}
