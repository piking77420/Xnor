#include "utils/octree_node.hpp"

#include "rendering/draw_gizmo.hpp"


using namespace XnorCore;

OctreeNode::OctreeNode(const Bound& boud, float_t minNodeSize) : m_Bound(boud) , m_MinNodeSize(minNodeSize)
{
    
}

void OctreeNode::DrawGizmo()
{
    DrawGizmo::Rectangle(m_Bound.center,m_Bound.extends);
}
