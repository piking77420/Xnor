#include "scene/component/test_component_animation.hpp"

#include "resource/resource_manager.hpp"
#include "scene/entity.hpp"

using namespace XnorCore;

void TestComponentAnimation::Begin()
{
    m_Renderer = GetEntity()->GetComponent<SkinnedMeshRenderer>();

    m_Montage.BindRenderer(m_Renderer);
    m_Montage.AddAnimation(0.f, size_t{0});
    m_Montage.AddAnimation(1.2f, ResourceManager::Get<Mesh>("assets/models/Coyote-Attack3.fbx")->GetAnimation(0));
    m_Montage.AddAnimation(3.f, size_t{0});
    m_Montage.AddAnimation(4.2f, ResourceManager::Get<Mesh>("assets/models/Coyote-Walking.fbx")->GetAnimation(0));

    m_Montage.Start();
}

void TestComponentAnimation::Update()
{
    m_Montage.Update();
}
