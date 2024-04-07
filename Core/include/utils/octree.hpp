#pragma once
#include "octree_node.hpp"
#include "scene/entity.hpp"

BEGIN_XNOR_CORE

class Octree
{
public:
    OctreeNode rootNode;

    bool_t draw;

    bool_t drawWithChild;
    
    XNOR_ENGINE Octree() = default;
    
    XNOR_ENGINE Octree(const List<Entity*>& list, float_t minNodeSize);

    XNOR_ENGINE ~Octree() = default;

    XNOR_ENGINE void Draw();
    
    XNOR_ENGINE void Compute(const List<Entity*>& list);

private:
    float_t m_MinSize = 1.f;

    XNOR_ENGINE void ComputeRootBound(const List<Entity*>& list);
};

END_XNOR_CORE