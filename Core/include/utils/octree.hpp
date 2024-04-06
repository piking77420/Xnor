#pragma once
#include "octree_node.hpp"
#include "scene/entity.hpp"

BEGIN_XNOR_CORE
class XNOR_ENGINE Octree
{
public:
    OctreeNode rootNode;

    bool_t draw;

    Octree() = default;
    
    Octree(const List<Entity*>& list, float_t minNodeSize);

    ~Octree() = default;

    void Draw();
    
    void Compute(const List<Entity*>& list);

private:
    float_t m_MinSize = 1.f;

};

END_XNOR_CORE