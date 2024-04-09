#pragma once

#include "core.hpp"
#include "data_structure/object_bounding.hpp"
#include "rendering/draw_gizmo.hpp"

BEGIN_XNOR_CORE


template<class T>
class OctreeNode
{
public:
    enum Octans : uint8_t
    {
        Q1 = 0x01, // 0b00000001
        Q2 = 0x02, // 0b00000010
        Q3 = 0x04, // 0b00000100
        Q4 = 0x08, // 0b00001000
        Q5 = 0x10, // 0b00010000
        Q6 = 0x20, // 0b00100000
        Q7 = 0x40, // 0b01000000
        Q8 = 0x80, // 0b10000000
    };

    DEFAULT_COPY_MOVE_OPERATIONS(OctreeNode)

    void AddObject(ObjectBounding<T>& objectBounding);
   

    OctreeNode(ObjectBounding<T> objectBounding);
    
    OctreeNode() = default; 

    ~OctreeNode()
    {
        for (size_t i = 0; i < m_Child.size(); i++)
        {
            if (m_ActiveOctans & (1 << i))
            {
                delete m_Child[i];
                m_Child[i] = nullptr;
            }
        }
    }

    void CreateBoundChild(Octans octans, Bound* outBound);

    void Draw();

    Bound& GetBound();

   
private:
    int32_t m_ActiveOctans = 0;
    std::array<OctreeNode*,8> m_Child;
    OctreeNode* m_Parent = nullptr;
    
    ObjectBounding<T> m_ObjectBounding;
    
    void DivideAndAdd(ObjectBounding<T>& objectBounding);

};


template <class T>
void OctreeNode<T>::AddObject(ObjectBounding<T>& objectBounding)
{
    DivideAndAdd(objectBounding);
}

template <class T>
OctreeNode<T>::OctreeNode(ObjectBounding<T> objectBounding)
: m_ObjectBounding(objectBounding)
{
        
}


template <class T>
void OctreeNode<T>::CreateBoundChild(Octans octans, Bound* outBound)
{
    
    const float_t quarter = m_ObjectBounding.bound.GetSize().x * 0.25f;
    const Vector3 childLength = m_ObjectBounding.bound.GetSize() * 0.5f;
    const Vector3 childSize = Vector3(childLength);
    const Vector3 center = m_ObjectBounding.bound.center;
    
    switch (octans)
    {
        case Q1:
            *outBound = Bound(center + Vector3(-quarter,quarter,-quarter), childSize ); 
            break;
            
        case Q2:
            *outBound = Bound(center + Vector3(quarter,quarter,-quarter), childSize ); 
            break;
            
        case Q3:
            *outBound = Bound(center + Vector3(-quarter,quarter,quarter), childSize ); 
            break;
            
        case Q4:
            *outBound = Bound(center + Vector3(quarter,quarter,quarter), childSize ); 
            break;
            
        case Q5:
            *outBound = Bound(center + Vector3(-quarter,-quarter,-quarter), childSize ); 
            break;
            
        case Q6:
            *outBound = Bound(center + Vector3(quarter,-quarter,-quarter), childSize ); 
            break;
            
        case Q7:
            *outBound = Bound(center + Vector3(-quarter,-quarter,quarter), childSize ); 
            break;
            
        case Q8:
            *outBound = Bound(center + Vector3(quarter,-quarter,quarter), childSize );
            break;
            
    }
}

template <class T>
void OctreeNode<T>::Draw()
{
    Colorf color = Colorf::Green();
    for (size_t i = 0; i < m_Child.size(); i++)
    {
        if (m_ActiveOctans & (1 << i))
        {
            if (m_Child[i]->m_ObjectBounding.handle)
            {
                color = Colorf::Blue();
                break;
            }
        }
    }
    DrawGizmo::Rectangle(m_ObjectBounding.bound.center, m_ObjectBounding.bound.extents, m_ObjectBounding.handle == nullptr ? color : Colorf::Red());

    for (size_t i = 0; i < m_Child.size(); i++)
    {
        if (m_ActiveOctans & (1 << i))
        {
             m_Child[i]->Draw();
        }
    }
}

template <class T>
Bound& OctreeNode<T>::GetBound()
{
    return m_ObjectBounding.bound;
}

template <class T>
void OctreeNode<T>::DivideAndAdd(ObjectBounding<T>& objectBounding)
{
    // END CONDITION Smallest value the render
    
    if (m_ObjectBounding.bound.GetSize().x < 1.f)
    {
        m_ObjectBounding = objectBounding;
        return;
    }

    for (size_t i = 0 ; i < 8; i++)
    {
        uint32_t current = (0 | (1 << i));
        Bound bound;
        CreateBoundChild(static_cast<Octans>(current),&bound);
        
        if (bound.Countains(objectBounding.bound))
        {
            m_ActiveOctans = (m_ActiveOctans | (1 << i));
            ObjectBounding<T> childrenData;
            childrenData.bound = bound;
            // Steal the handle
            m_Child[i] = new OctreeNode(childrenData);
            m_Child[i]->m_Parent = this;
            m_Child[i]->AddObject(objectBounding);
        }
    }
    
}

END_XNOR_CORE
