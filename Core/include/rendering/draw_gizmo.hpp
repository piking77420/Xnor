#pragma once

#include <list>

#include "core.hpp"
#include "Maths/vector3.hpp"
#include "utils/color.hpp"

BEGIN_XNOR_CORE
class DrawGizmo
{
public:

    STATIC_CLASS(DrawGizmo)

    static void DrawSphere(Vector3 position, float_t radius, ColorRgb color);
    
    static void DrawRectancgle(Vector3 position, Vector3 size, ColorRgb color);
    
    static void DrawCapsule(Vector3 position, float_t height, float_t radius, ColorRgb color);

private:
 
};

END_XNOR_CORE