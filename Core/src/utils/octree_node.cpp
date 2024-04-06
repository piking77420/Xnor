#include "utils/octree_node.hpp"

#include "rendering/draw_gizmo.hpp"


using namespace XnorCore;

OctreeNode::OctreeNode(const Bound& boud, float_t minNodeSize) : m_Bound(boud) , m_MinNodeSize(minNodeSize)
{
    const float_t quarter = m_Bound.size.x * 0.25f;
    const float_t childLength = m_Bound.size.x * 0.5f;
    const Vector3 childSize = Vector3(childLength);
    
    m_ChildBound[TopLeftFront] = Bound(m_Bound.center + Vector3(-quarter,quarter,-quarter),childLength,childLength,childLength);
    
    m_ChildBound[TopRightFront] = Bound(m_Bound.center + Vector3(quarter,quarter,-quarter),childLength,childLength,childLength);
    
    m_ChildBound[BottomRightFront] = Bound(m_Bound.center + Vector3(-quarter,quarter,quarter),childLength,childLength,childLength);
    
    m_ChildBound[BottomLeftFront] = Bound(m_Bound.center + Vector3(quarter,quarter,quarter),childLength,childLength,childLength);
    
    m_ChildBound[TopLeftBottom] = Bound(m_Bound.center + Vector3(-quarter,-quarter,-quarter),childLength,childLength,childLength);
    
    m_ChildBound[TopRightBottom] = Bound(m_Bound.center + Vector3(quarter,-quarter,-quarter),childLength,childLength,childLength);
    
    m_ChildBound[BottomRightBack] = Bound(m_Bound.center + Vector3(-quarter,-quarter,quarter),childLength,childLength,childLength);

    m_ChildBound[BottomLeftBack] = Bound(m_Bound.center + Vector3(quarter,-quarter,quarter),childLength,childLength,childLength);

}

OctreeNode::~OctreeNode()
{
    if (m_Childs != nullptr)
    for (size_t i = 0; i < m_Childs->size(); i++)
    {
        delete m_Childs->at(i);
    }

    delete m_Childs;
}

void OctreeNode::AddObject(const Bound& bound)
{
    DivideAndAdd(bound);
}


void OctreeNode::DrawGizmo()
{
    DrawGizmo::Rectangle(m_Bound.center,m_Bound.size);

    if (m_Childs != nullptr)
    for (size_t i = 0; i < m_Childs->size(); i++)
    {
        if (!m_Childs->at(i))
            continue;
        
        m_Childs->at(i)->DrawGizmo();
    }
}

void OctreeNode::DivideAndAdd(const Bound& bound)
{
    if (m_Bound.size.x <= m_MinNodeSize)
    {
        
        return;
    }

    if (m_Childs == nullptr)
        m_Childs = new std::array<OctreeNode*,OctoTreeNodeChildArraySize>();
    
    bool_t dividing = false;

    for (size_t i = 0; i < 8 ; i++)
    {
        
        if (m_Childs->at(i) == nullptr)
            m_Childs->at(i) = new OctreeNode(m_ChildBound[i],m_MinNodeSize);

        if (m_ChildBound[i].Intersect(bound))
        {
            dividing = true;
            m_Childs->at(i)->AddObject(bound);
        }
    }

    if (dividing == false)
    {
        for (size_t i = 0; i < m_Childs->size(); i++)
        {
            delete m_Childs->at(i);
        }

        delete m_Childs;
        m_Childs = nullptr;
    }
}
