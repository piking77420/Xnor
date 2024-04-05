#include "editing/draw_gizmo.hpp"

using namespace XnorEditor;

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
    XnorCore::RenderPassBeginInfo renderPassBeginInfo =
    {
        .frameBuffer = viewport.frameBuffer,
        .renderAreaOffset = { 0 , 0 },
        .renderAreaExtent = viewport.viewPortSize,
        .clearBufferFlags = XnorCore::BufferFlag::None,
        .clearColor = Vector4::Zero()
    };
    m_RenderPass.BeginRenderPass(renderPassBeginInfo);
    
    DrawRectangle();
    DrawSphere();
    DrawCapsule();
    
    m_RenderPass.EndRenderPass();
}

void DrawGizmo::DrawRectangle()
{
}

void DrawGizmo::DrawSphere()
{
}

void DrawGizmo::DrawCapsule()
{
}

