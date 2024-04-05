#pragma once

#include <vector>

#include "definitions.hpp"
#include "Maths/vector3.hpp"
#include "rendering/render_pass.hpp"
#include "utils/color.hpp"
#include "utils/pointer.hpp"

namespace XnorCore
{
    class Shader;
    class Viewport;
}

BEGIN_XNOR_EDITOR

class DrawGizmo
{
private:
    static constexpr size_t BaseReserveGizmoVector = 100;

    
    struct GizmoSphere
    {
        Vector3 position;
        float_t radius;
        XnorCore::ColorRgb color;
    };

    struct GizmoRectangle
    {
        Vector3 position;
        Vector3 size;
        XnorCore::ColorRgb color;
    };

    struct GizmoCapsule
    {
        Vector3 position;
        float_t height;
        float_t radius;
        XnorCore::ColorRgb color;
    };
    
    static inline std::vector<GizmoSphere> m_GizmoSphereVector {std::vector<GizmoSphere>(BaseReserveGizmoVector)};
    static inline std::vector<GizmoRectangle> m_GizmoRectangleVector {std::vector<GizmoRectangle>(BaseReserveGizmoVector)};
    static inline std::vector<GizmoCapsule> m_GizmoCapsuleVector {std::vector<GizmoCapsule>(BaseReserveGizmoVector)};
    
public:
    DrawGizmo() = default;

    ~DrawGizmo() = default;
    
    static void Sphere(Vector3 position, float_t radius = 1.f, XnorCore::ColorRgb color = XnorCore::ColorRgb::Green());
    
    static void Rectancgle(Vector3 position, Vector3 size, XnorCore::ColorRgb color = XnorCore::ColorRgb::Green());
    
    static void Capsule(Vector3 position, float_t height, float_t radius, XnorCore::ColorRgb color = XnorCore::ColorRgb::Green());

    void DrawGizmos(const XnorCore::Viewport& viewport);

private:
    XnorCore::Pointer<XnorCore::Shader> m_GizmoShader;

    XnorCore::RenderPass m_RenderPass;
};

END_XNOR_EDITOR
