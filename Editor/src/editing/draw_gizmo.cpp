#include "editing/draw_gizmo.hpp"

#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"

using namespace XnorEditor;

DrawGizmo::DrawGizmo()
{
    m_Sphere = XnorCore::ResourceManager::Get<XnorCore::Model>("sphere.obj");
    m_Cube = XnorCore::ResourceManager::Get<XnorCore::Model>("cube.obj");

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

void DrawGizmo::DrawGizmos(const XnorCore::Viewport& viewport)
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
    XnorCore::Rhi::SetPolygonMode(XnorCore::PolygonFace::PolygonFace::FrontAndBack, XnorCore::PolygonMode::Line);


    DrawRectangle();
    DrawSphere();
    DrawCapsule();
    
    m_RenderPass.EndRenderPass();
    XnorCore::Rhi::SetPolygonMode(XnorCore::PolygonFace::PolygonFace::FrontAndBack, XnorCore::PolygonMode::Fill);
    Clear();
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
    for (GizmoCapsule& gizmo : m_GizmoCapsuleVector)
    {
        
    }
}

void DrawGizmo::Clear()
{
    m_GizmoSphereVector.clear();
    m_GizmoRectangleVector.clear();
    m_GizmoCapsuleVector.clear();
}


