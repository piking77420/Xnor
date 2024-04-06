#pragma once

#include "core.hpp"

#include <array>

#include "bound.hpp"
#include "Maths/vector3.hpp"

BEGIN_XNOR_CORE

class OctreeNode
{
private:
    XNOR_ENGINE static inline constexpr size_t OctoTreeNodeChildArraySize = 8;
public:
    enum Side
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

    XNOR_ENGINE OctreeNode() = default;

    XNOR_ENGINE OctreeNode(const Bound& boud,float_t minNodeSize);

    XNOR_ENGINE ~OctreeNode(); 
    
    XNOR_ENGINE void AddObject(const Bound& bound);
    
    XNOR_ENGINE void DrawGizmo();

private:
    Bound m_Bound;
    float_t m_MinNodeSize = 0.f;

    XNOR_ENGINE void DivideAndAdd(const Bound& bound);
    
    std::array<OctreeNode*,OctoTreeNodeChildArraySize>* m_Childs = nullptr;
    std::array<Bound,OctoTreeNodeChildArraySize> m_ChildBound;

};


END_XNOR_CORE