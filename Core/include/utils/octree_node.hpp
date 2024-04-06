#pragma once

#include "core.hpp"

#include <array>

#include "bound.hpp"
#include "Maths/vector3.hpp"

BEGIN_XNOR_CORE

class OctreeNode
{
public:
    enum class Side
    {
        TopLeftFront,
        TopRightFront,
        BottomRightFront,
        BottomLeftFront,
        TopLeftBottom,
        TopRightBottom,
        BottomRightBack,
        BottomLeftBack,
    };

    DEFAULT_COPY_MOVE_OPERATIONS(OctreeNode)

    OctreeNode() = default;

    OctreeNode(const Bound& boud,float_t minNodeSize);
    
    ~OctreeNode() = default; 
    
    void DrawGizmo();

private:
    Bound m_Bound;
    float_t m_MinNodeSize = 0.f;
};

END_XNOR_CORE