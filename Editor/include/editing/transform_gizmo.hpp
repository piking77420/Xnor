#pragma once


#include "core.hpp"
#include "definitions.hpp"
#include "Maths/matrix.hpp"

#include <ImGui/imgui.h>
#include<ImguiGizmo/ImGuizmo.h>

#include "transform.hpp"
#include "rendering/camera.hpp"

BEGIN_XNOR_EDITOR

class TransfromGizmo
{
public:
    DEFAULT_COPY_MOVE_OPERATIONS(TransfromGizmo)

    TransfromGizmo() = default;

    ~TransfromGizmo() = default;

    void SetRendering(const XnorCore::Camera& camera, Vector2 windowPos, Vector2i windowSize);

    bool_t Manipulate(XnorCore::Transform& transform);

private:
    ImGuizmo::OPERATION m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
    ImGuizmo::MODE m_CurrentGizmoMode = ImGuizmo::WORLD;
    
    Matrix m_View;
    Matrix m_Projection;
};

END_XNOR_EDITOR
