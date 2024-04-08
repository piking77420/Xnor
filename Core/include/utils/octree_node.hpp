#pragma once

#include "core.hpp"

#include <array>

#include "bound.hpp"
#include "Maths/vector3.hpp"
#include "rendering/draw_gizmo.hpp"

BEGIN_XNOR_CORE


template <class T>
struct OctreeNodeData
{
    T handle;
    Bound bound;

    DEFAULT_COPY_MOVE_OPERATIONS(OctreeNodeData<T>)
    
    OctreeNodeData()
    {
        if constexpr (Meta::IsPointer<T>)
        {
            handle = nullptr;
        }         
    }
};

template <class T>
class OctreeNode
{
private:
    XNOR_ENGINE static inline constexpr size_t OctTreeNodeChildArraySize = 8;
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

    float_t minNodeSize = 0.f;

    OctreeNodeData<T> nodeData;

    Colorf colorf = Colorf::Green();
    
    DEFAULT_COPY_MOVE_OPERATIONS(OctreeNode)

    OctreeNode(OctreeNodeData<T>& octreeNodeData, float_t newminNodeSize);
    
    OctreeNode();
    
    ~OctreeNode();

    void Update(OctreeNodeData<T>& currentNodeData);
    
    void AddObject(OctreeNodeData<T>& currentNodeData);
    
    void DrawGizmo();

    void DrawGizmoWithChild();

private:

    std::array<OctreeNode*,OctTreeNodeChildArraySize> m_Childs;
    std::array<Bound,OctTreeNodeChildArraySize> m_ChildBound;
    
    void DivideAndAdd(OctreeNodeData<T>& nodeData);

    bool_t IsEmpty();

    void InitArray();

};

template <class T>
OctreeNode<T>::OctreeNode(OctreeNodeData<T>& octreeNodeData, float_t newminNodeSize) : minNodeSize(newminNodeSize)
{
    InitArray();
    Update(octreeNodeData);
}

template <class T>
OctreeNode<T>::OctreeNode()
{
    InitArray();
}

template <class T>
OctreeNode<T>::~OctreeNode()
{
    if(IsEmpty())
        return;
    
     for (size_t i = 0; i < 8; i++)
     {
         if (m_Childs.at(i) == nullptr)
             continue;
         
         delete m_Childs.at(i);
     }
}

template <class T>
void OctreeNode<T>::Update(OctreeNodeData<T>& octreeNodeData)
{
    nodeData.bound = octreeNodeData.bound;
    nodeData.handle = octreeNodeData.handle;
    octreeNodeData.handle = nullptr;
    
    
    const float_t quarter = nodeData.bound.GetSize().x * 0.25f;
    const float_t childLength = nodeData.bound.GetSize().x * 0.5f;
    const Vector3 childSize = Vector3(childLength);
    
    m_ChildBound[TopLeftFront] = Bound(nodeData.bound.center + Vector3(-quarter,quarter,-quarter), childSize );
    
    m_ChildBound[TopRightFront] = Bound(nodeData.bound.center + Vector3(quarter,quarter,-quarter), childSize );
    
    m_ChildBound[BottomRightFront] = Bound(nodeData.bound.center + Vector3(-quarter,quarter,quarter), childSize );
    
    m_ChildBound[BottomLeftFront] = Bound(nodeData.bound.center + Vector3(quarter,quarter,quarter), childSize );
    
    m_ChildBound[TopLeftBottom] = Bound(nodeData.bound.center + Vector3(-quarter,-quarter,-quarter), childSize );
    
    m_ChildBound[TopRightBottom] = Bound(nodeData.bound.center + Vector3(quarter,-quarter,-quarter), childSize );
    
    m_ChildBound[BottomRightBack] = Bound(nodeData.bound.center + Vector3(-quarter,-quarter,quarter), childSize );

    m_ChildBound[BottomLeftBack] = Bound(nodeData.bound.center + Vector3(quarter,-quarter,quarter), childSize );
}

template <class T>
void OctreeNode<T>::AddObject(OctreeNodeData<T>& currentNodeData)
{
    DivideAndAdd(currentNodeData);
}

template <class T>
void OctreeNode<T>::DrawGizmo()
{
    DrawGizmo::Rectangle(nodeData.bound.center, nodeData.bound.extents,colorf);
}

template <class T>
void OctreeNode<T>::DrawGizmoWithChild()
{
    DrawGizmo();
    
    for (size_t i = 0; i < m_Childs.size(); i++)
    {
        if (!m_Childs.at(i))
            continue;
        
        m_Childs.at(i)->DrawGizmoWithChild();
    }
}

template <class T>
void OctreeNode<T>::DivideAndAdd(OctreeNodeData<T>& currentNodeData)
{

    if (nodeData.bound.GetSize().x <= minNodeSize)
    {
        nodeData.handle = currentNodeData.handle;
        currentNodeData = nullptr;
        colorf = Colorf::Red();
        return;
    }

    
    bool_t dividing = false;

    for (size_t i = 0; i < OctTreeNodeChildArraySize ; i++)
    {
        
        if (m_Childs.at(i) == nullptr)
        {
            OctreeNodeData<T> newNode;
            newNode.handle = currentNodeData.handle;
            newNode.bound = m_ChildBound.at(i);
            currentNodeData.handle = nullptr;
            m_Childs.at(i)->colorf = Colorf::Yellow();
            m_Childs.at(i) = new OctreeNode(newNode, minNodeSize);
        }
        
        // Empty quad
        if (m_Childs.at(i)->nodeData.bound.Intersect(currentNodeData.bound))
        {
            dividing = true;
            m_Childs.at(i)->colorf = Colorf::Blue();
            m_Childs.at(i)->AddObject(currentNodeData);
        }
      
      
    }

    if (dividing == false || nodeData.handle == nullptr)
    {
        for (size_t i = 0; i < m_Childs.size(); i++)
        {
            delete m_Childs.at(i);
            m_Childs.at(i) = nullptr;
        }
    }
    
}

template <class T>
bool_t OctreeNode<T>::IsEmpty()
{
    for (const OctreeNode* octreeNode : m_Childs)
    {
        if (octreeNode != nullptr)
            return false;
    }

    return true;
}

template <class T>
void OctreeNode<T>::InitArray()
{
    for (OctreeNode*& octreeNode : m_Childs)
    {
        octreeNode = nullptr;
    }
}


END_XNOR_CORE
