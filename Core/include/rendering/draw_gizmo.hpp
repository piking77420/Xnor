#pragma once

#include <vector>

#include <Maths/vector3.hpp>
#include "rendering/render_pass.hpp"
#include "rendering/viewport.hpp"
#include "utils/color.hpp"
#include "utils/pointer.hpp"


BEGIN_XNOR_CORE

class DrawGizmo
{
private:
    struct GizmoSphere
    {
        Vector3 position;
        float_t radius;
        Colorf color;
    };

    struct GizmoRectangle
    {
        Vector3 position;
        Vector3 size;
        Colorf color;
    };

    struct GizmoCapsule
    {
        Vector3 position;
        float_t height;
        float_t radius;
        Colorf color;
    };
    
    XNOR_ENGINE static inline std::vector<GizmoSphere> m_GizmoSphereVector;
    XNOR_ENGINE static inline std::vector<GizmoRectangle> m_GizmoRectangleVector;
    XNOR_ENGINE static inline std::vector<GizmoCapsule> m_GizmoCapsuleVector;
    
public:
    XNOR_ENGINE DrawGizmo();

    XNOR_ENGINE ~DrawGizmo() = default;
    
    XNOR_ENGINE static void Sphere(Vector3 position, float_t radius = 1.f, Colorf color = Colorf::Green());
    
    XNOR_ENGINE static void Rectangle(Vector3 position, Vector3 size, Colorf color = Colorf::Green());
    
    XNOR_ENGINE static void Capsule(Vector3 position, float_t height, float_t radius, Colorf color = Colorf::Green());

    XNOR_ENGINE void DrawGizmos(const Viewport& viewport, const Entity* selectedEntity);


private:
    Pointer<XnorCore::Shader> m_GizmoShader;

    Pointer<XnorCore::Mesh> m_Sphere;

    Pointer<XnorCore::Mesh> m_Cube;

    Pointer<XnorCore::Mesh> m_Capsule;
    
    RenderPass m_RenderPass;

    ModelUniformData modelData;
    
    XNOR_ENGINE void DrawRectangle();

    XNOR_ENGINE void DrawSphere();

    XNOR_ENGINE void DrawCapsule();
    
    XNOR_ENGINE void Clear();
    
    XNOR_ENGINE void DrawCollider(const Entity& entity);

};

END_XNOR_CORE
