#include "utils/octree_node.hpp"

#include "rendering/draw_gizmo.hpp"


using namespace XnorCore;

OctreeNode::OctreeNode()
{
    InitArray();
}

OctreeNode::OctreeNode(const Bound& boud, float_t minNodeSize) : m_Bound(boud) , m_MinNodeSize(minNodeSize)
{
    InitArray();
    
    const float_t quarter = m_Bound.GetSize().x * 0.25f;
    const float_t childLength = m_Bound.GetSize().x * 0.5f;
    const Vector3 childSize = Vector3(childLength);
    
    m_ChildBound[TopLeftFront] = Bound(m_Bound.center + Vector3(-quarter,quarter,-quarter), childSize );
    
    m_ChildBound[TopRightFront] = Bound(m_Bound.center + Vector3(quarter,quarter,-quarter), childSize );
    
    m_ChildBound[BottomRightFront] = Bound(m_Bound.center + Vector3(-quarter,quarter,quarter), childSize );
    
    m_ChildBound[BottomLeftFront] = Bound(m_Bound.center + Vector3(quarter,quarter,quarter), childSize );
    
    m_ChildBound[TopLeftBottom] = Bound(m_Bound.center + Vector3(-quarter,-quarter,-quarter), childSize );
    
    m_ChildBound[TopRightBottom] = Bound(m_Bound.center + Vector3(quarter,-quarter,-quarter), childSize );
    
    m_ChildBound[BottomRightBack] = Bound(m_Bound.center + Vector3(-quarter,-quarter,quarter), childSize );

    m_ChildBound[BottomLeftBack] = Bound(m_Bound.center + Vector3(quarter,-quarter,quarter), childSize );

}

OctreeNode::~OctreeNode()
{
    if (IsEmpty())
        return;
    
    for (size_t i = 0; i < 8; i++)
    {
        delete m_Childs.at(i);
    }
}

void OctreeNode::AddObject(const Bound& bound)
{
    DivideAndAdd(bound);
}


void OctreeNode::DrawGizmo()
{
    DrawGizmo::Rectangle(m_Bound.center, m_Bound.extents);
}

void OctreeNode::DrawGizmoWithChild()
{
    DrawGizmo::Rectangle(m_Bound.center, m_Bound.extents);
    
    for (size_t i = 0; i < m_Childs.size(); i++)
    {
        if (!m_Childs.at(i))
                continue;
        
        m_Childs.at(i)->DrawGizmoWithChild();
    }
    
}

void OctreeNode::DivideAndAdd(const Bound& bound)
{
    if (m_Bound.GetSize().x <= m_MinNodeSize)
    {
        return;
    }
    
    bool_t dividing = false;

    for (size_t i = 0; i < 8 ; i++)
    {
        
        if (m_Childs.at(i) == nullptr)
            m_Childs.at(i) = new OctreeNode(m_ChildBound.at(i),m_MinNodeSize);

        if (m_Childs.at(i)->m_Bound.Intersect(bound))
        {
            dividing = true;
            m_Childs.at(i)->AddObject(bound);
        }
    }

    if (dividing == false)
    {
        for (size_t i = 0; i < m_Childs.size(); i++)
        {
            delete m_Childs.at(i);
        }
    }
}

bool_t OctreeNode::IsEmpty()
{
    for (const OctreeNode* octreeNode : m_Childs)
    {
        if (octreeNode != nullptr)
            return false;
    }

    return true;
}

void OctreeNode::InitArray()
{
    for (OctreeNode*& octreeNode : m_Childs)
    {
        octreeNode = nullptr;
    }
}
