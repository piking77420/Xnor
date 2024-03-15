#include "physics/components/mesh_collider.hpp"

#include "physics/physics_world.hpp"
#include "scene/entity.hpp"
#include "scene/component/mesh_renderer.hpp"

using namespace XnorCore;

REFLECTABLE_IMPL_CPP(MeshCollider)

void MeshCollider::Begin()
{
    MeshRenderer* renderer;
    if (!entity->TryGetComponent<MeshRenderer>(&renderer))
    {
        Logger::LogError("A mesh collider component must have a mesh renderer");
        return;
    }

    const Transform& t = entity->transform;
    m_BodyId = PhysicsWorld::CreateConvexHull(this, t.GetPosition(), t.GetRotation(), t.GetScale(), renderer->model->GetVertices(), m_IsTrigger, m_IsStatic);
}

void MeshCollider::Update()
{
    Collider::Update();
}
