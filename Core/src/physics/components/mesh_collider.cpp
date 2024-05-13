#include "physics/components/mesh_collider.hpp"

#include <utils/logger.hpp>

#include "physics/physics_world.hpp"
#include "scene/entity.hpp"
#include "scene/component/mesh_renderer.hpp"

using namespace XnorCore;


void MeshCollider::Begin()
{
    MeshRenderer* renderer;
    if (!entity->TryGetComponent<MeshRenderer>(&renderer))
    {
        Logger::LogError("A mesh collider component must have a mesh renderer");
        return;
    }

    const Transform& t = entity->transform;

    const PhysicsWorld::BodyCreationInfo info = {
        .collider = this,
        .position = t.GetPosition(),
        .rotation = t.GetRotation(),
        .scaling = t.GetScale(),
        .isTrigger = m_IsTrigger,
        .isStatic = m_IsStatic
    };

    // TODO do it for each models
    m_BodyId = PhysicsWorld::CreateConvexHull(info, renderer->mesh->models[0]->GetVertices());
}

void MeshCollider::Update()
{
    Collider::Update();
}
