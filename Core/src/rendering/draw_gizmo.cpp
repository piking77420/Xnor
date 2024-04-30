#include "rendering/draw_gizmo.hpp"

#include "physics/components/box_collider.hpp"
#include "physics/components/capsule_collider.hpp"
#include "physics/components/collider.hpp"
#include "physics/components/sphere_collider.hpp"
#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"

using namespace XnorCore;

DrawGizmo::DrawGizmo()
{
     m_Sphere = ResourceManager::Get<Model>("assets/models/sphere.obj");
     m_Cube = ResourceManager::Get<Model>("assets/models/cube.obj");
     m_Capsule = ResourceManager::Get<Model>("assets/models/capsule.obj");
    m_GizmoShader = ResourceManager::Get<Shader>("gizmo_shader");
}

void DrawGizmo::Sphere(Vector3 position, float_t radius, Color color)
{
    m_GizmoSphereVector.push_back({ position, radius, color });
}

void DrawGizmo::Rectangle(Vector3 position, Vector3 size, Color color)
{
    m_GizmoRectangleVector.push_back({ position, size, color });
}

void DrawGizmo::Capsule(Vector3 position, float_t height, float_t radius, Color color)
{
    m_GizmoCapsuleVector.push_back({ position, height, radius, color });
}

void DrawGizmo::DrawGizmos(const Viewport& viewport, const Entity* selectedEntity)
{
    const RenderPassBeginInfo renderPassBeginInfo =
    {
        .frameBuffer = viewport.frameBuffer,
        .renderAreaOffset = { 0 , 0 },
        .renderAreaExtent = viewport.viewPortSize,
        .clearBufferFlags = BufferFlag::None,
        .clearColor = Vector4::Zero()
    };
    m_RenderPass.BeginRenderPass(renderPassBeginInfo);
    m_GizmoShader->Use();
    Rhi::SetPolygonMode(PolygonFace::PolygonFace::FrontAndBack, PolygonMode::Line);

    if (selectedEntity != nullptr)
    DrawCollider(*selectedEntity);

    DrawRectangle();
    DrawSphere();
    DrawCapsule();
    
    m_RenderPass.EndRenderPass();
    Rhi::SetPolygonMode(PolygonFace::PolygonFace::FrontAndBack, PolygonMode::Fill);
    m_GizmoShader->Unuse();
    
    Clear();
}

void DrawGizmo::DrawCollider(const Entity& entity)
{
    std::vector<const Collider*> entityCollider;
    entity.GetComponents<Collider>(&entityCollider);

    if (entityCollider.empty())
        return;

    Pointer<Model> model;
    const Collider* collider = entityCollider[0];
    Matrix colliderMatrix;
    
    if (dynamic_cast<const SphereCollider*>(collider))
    {
        model = m_Sphere;
        const float_t radius = reinterpret_cast<const SphereCollider*>(collider)->radius;
        colliderMatrix = Matrix::Trs(Vector3::Zero(), Quaternion::Identity(), Vector3(radius));
    }
    else if( dynamic_cast<const BoxCollider*>(collider))
    {
        model = m_Cube;
        const BoxCollider* boxCollider = reinterpret_cast<const BoxCollider*>(collider);
        colliderMatrix = Matrix::Trs(boxCollider->center, Quaternion::Identity(), boxCollider->size);
    }
    else if(dynamic_cast<const CapsuleCollider*>(collider))
    {
        model = m_Capsule;
        const CapsuleCollider* capsuleCollider = reinterpret_cast<const CapsuleCollider*>(collider);
        const Vector3 capsuleScale = Vector3(capsuleCollider->radius,capsuleCollider->height,capsuleCollider->radius);
        colliderMatrix = Matrix::Trs(Vector3::Zero(), Quaternion::Identity(), capsuleScale);
    }
    else
    {
        return;
    }
    modelData.model = entity.transform.worldMatrix * colliderMatrix;
    Rhi::UpdateModelUniform(modelData);
    Rhi::DrawModel(DrawMode::Triangles, model->GetId());
    
    
}

void DrawGizmo::DrawRectangle()
{
    for (GizmoRectangle& gizmo : m_GizmoRectangleVector)
    {
        m_GizmoShader->SetVec3("color",static_cast<Vector3>(gizmo.color));
        modelData.model = Matrix::Trs(gizmo.position, Quaternion::Identity(), Vector3(gizmo.size));
        Rhi::UpdateModelUniform(modelData);
        Rhi::DrawModel(DrawMode::Triangles, m_Cube->GetId());
    }
}

void DrawGizmo::DrawSphere()
{

    for (GizmoSphere& gizmo : m_GizmoSphereVector)
    {
        //m_GizmoShader->SetVec3("color",static_cast<Vector3>(gizmo.color));
        modelData.model = Matrix::Trs(gizmo.position, Quaternion::Identity(), Vector3(gizmo.radius));
        Rhi::UpdateModelUniform(modelData);
        Rhi::DrawModel(DrawMode::Triangles, m_Sphere->GetId());
    }
    
}

void DrawGizmo::DrawCapsule()
{
    for ([[maybe_unused]] GizmoCapsule& gizmo : m_GizmoCapsuleVector)
    {

    }
}

void DrawGizmo::Clear()
{
    m_GizmoSphereVector.clear();
    m_GizmoRectangleVector.clear();
    m_GizmoCapsuleVector.clear();
}


