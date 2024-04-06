#include "editing/draw_gizmo.hpp"

#include "physics/components/box_collider.hpp"
#include "physics/components/capsule_collider.hpp"
#include "physics/components/collider.hpp"
#include "physics/components/sphere_collider.hpp"
#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"

using namespace XnorEditor;

DrawGizmo::DrawGizmo()
{
     m_Sphere = XnorCore::ResourceManager::Get<XnorCore::Model>("assets/models/sphere.obj");
     m_Cube = XnorCore::ResourceManager::Get<XnorCore::Model>("assets/models/cube.obj");
     m_Capsule = XnorCore::ResourceManager::Get<XnorCore::Model>("assets/models/capsule.obj");
    m_GizmoShader = XnorCore::ResourceManager::Get<XnorCore::Shader>("gizmo_shader");
}

void DrawGizmo::Sphere(Vector3 position, float_t radius, XnorCore::ColorRgb color)
{
    m_GizmoSphereVector.push_back({ position, radius, color });
}

void DrawGizmo::Rectancgle(Vector3 position, Vector3 size, XnorCore::ColorRgb color)
{
    m_GizmoRectangleVector.push_back({ position, size, color });
}

void DrawGizmo::Capsule(Vector3 position, float_t height, float_t radius, XnorCore::ColorRgb color)
{
    m_GizmoCapsuleVector.push_back({ position, height, radius, color });
}

void DrawGizmo::DrawGizmos(const XnorCore::Viewport& viewport, const XnorCore::Entity* selectedEntity)
{
    const XnorCore::RenderPassBeginInfo renderPassBeginInfo =
    {
        .frameBuffer = viewport.frameBuffer,
        .renderAreaOffset = { 0 , 0 },
        .renderAreaExtent = viewport.viewPortSize,
        .clearBufferFlags = XnorCore::BufferFlag::None,
        .clearColor = Vector4::Zero()
    };
    m_RenderPass.BeginRenderPass(renderPassBeginInfo);
    m_GizmoShader->Use();
    XnorCore::Rhi::SetPolygonMode(XnorCore::PolygonFace::PolygonFace::FrontAndBack, XnorCore::PolygonMode::Line);

    if (selectedEntity != nullptr)
    DrawCollider(*selectedEntity);

    DrawRectangle();
    DrawSphere();
    DrawCapsule();
    
    m_RenderPass.EndRenderPass();
    XnorCore::Rhi::SetPolygonMode(XnorCore::PolygonFace::PolygonFace::FrontAndBack, XnorCore::PolygonMode::Fill);
    m_GizmoShader->Unuse();
    
    Clear();
}

void DrawGizmo::DrawCollider(const XnorCore::Entity& entity)
{
    std::vector<const XnorCore::Collider*> entityCollider;
    entity.GetComponents<XnorCore::Collider>(&entityCollider);

    if (entityCollider.empty())
        return;

    XnorCore::Pointer<XnorCore::Model> model;
    const XnorCore::Collider* collider = entityCollider[0];
    Matrix colliderMatrix;
    
    if (dynamic_cast<const XnorCore::SphereCollider*>(collider))
    {
        model = m_Sphere;
        const float_t radius = reinterpret_cast<const XnorCore::SphereCollider*>(collider)->radius;
        colliderMatrix = Matrix::Trs(Vector3::Zero(), Quaternion::Identity(), Vector3(radius));
    }
    else if( dynamic_cast<const XnorCore::BoxCollider*>(collider))
    {
        model = m_Cube;
        const XnorCore::BoxCollider* boxCollider = reinterpret_cast<const XnorCore::BoxCollider*>(collider);
        colliderMatrix = Matrix::Trs(boxCollider->center, Quaternion::Identity(), boxCollider->size);
    }
    else if(dynamic_cast<const XnorCore::CapsuleCollider*>(collider))
    {
        model = m_Capsule;
        const XnorCore::CapsuleCollider* capsuleCollider = reinterpret_cast<const XnorCore::CapsuleCollider*>(collider);
        const Vector3 capsuleScale = Vector3(capsuleCollider->radius,capsuleCollider->height,capsuleCollider->radius);
        colliderMatrix = Matrix::Trs(Vector3::Zero(), Quaternion::Identity(), capsuleScale);
    }
    else
    {
        return;
    }
    modelData.model = entity.transform.worldMatrix * colliderMatrix;
    XnorCore::Rhi::UpdateModelUniform(modelData);
    XnorCore::Rhi::DrawModel(model->GetId());
    
    
}

void DrawGizmo::DrawRectangle()
{
    for (GizmoSphere& gizmo : m_GizmoSphereVector)
    {
        modelData.model = Matrix::Trs(gizmo.position, Quaternion::Identity(), Vector3(gizmo.radius));
        XnorCore::Rhi::UpdateModelUniform(modelData);
        XnorCore::Rhi::DrawModel(m_Sphere->GetId());
    }
}

void DrawGizmo::DrawSphere()
{

    for (GizmoRectangle& gizmo : m_GizmoRectangleVector)
    {
        modelData.model = Matrix::Trs(gizmo.position, Quaternion::Identity(), Vector3(gizmo.size));
        XnorCore::Rhi::UpdateModelUniform(modelData);
        XnorCore::Rhi::DrawModel(m_Sphere->GetId());
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


