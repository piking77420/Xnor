#pragma once

#include <vector>

#include "definitions.hpp"
#include "Maths/vector3.hpp"
#include "rendering/render_pass.hpp"
#include "rendering/viewport.hpp"
#include "utils/color.hpp"
#include "utils/pointer.hpp"


BEGIN_XNOR_EDITOR

class DrawGizmo
{
private:
    
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
    
    static inline std::vector<GizmoSphere> m_GizmoSphereVector;
    static inline std::vector<GizmoRectangle> m_GizmoRectangleVector;
    static inline std::vector<GizmoCapsule> m_GizmoCapsuleVector;
    
public:
    DrawGizmo();

    ~DrawGizmo() = default;
    
    static void Sphere(Vector3 position, float_t radius = 1.f, XnorCore::ColorRgb color = XnorCore::ColorRgb::Green());
    
    static void Rectancgle(Vector3 position, Vector3 size, XnorCore::ColorRgb color = XnorCore::ColorRgb::Green());
    
    static void Capsule(Vector3 position, float_t height, float_t radius, XnorCore::ColorRgb color = XnorCore::ColorRgb::Green());

    void DrawGizmos(const XnorCore::Viewport& viewport, const XnorCore::Entity* selectedEntity);


private:
    XnorCore::Pointer<XnorCore::Shader> m_GizmoShader;

    XnorCore::Pointer<XnorCore::Model> m_Sphere;

    XnorCore::Pointer<XnorCore::Model> m_Cube;

    XnorCore::Pointer<XnorCore::Model> m_Capsule;
    
    XnorCore::RenderPass m_RenderPass;

    XnorCore::ModelUniformData modelData;
    
    void DrawRectangle();

    void DrawSphere();

    void DrawCapsule();
    
    void Clear();
    
    void DrawCollider(const XnorCore::Entity& entity);

};

END_XNOR_EDITOR
