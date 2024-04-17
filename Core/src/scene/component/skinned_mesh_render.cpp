#include "scene/component/skinned_mesh_render.hpp"

#include "rendering/animator.hpp"

using namespace XnorCore;

void SkinnedMeshRender::Begin()
{
    m_Animator.Start(mesh->animations[0]);
}

void SkinnedMeshRender::Update()
{
    m_Animator.Animate();
}

void SkinnedMeshRender::StartBlending(const Animation* target)
{
    m_TargetAnimator.Start(target);
    m_Animator.StartBlending(&m_TargetAnimator);
}

const List<Matrix>& SkinnedMeshRender::GetMatrices() const
{
    return m_Animator.GetMatrices();
}
