#pragma once


#include <rendering/draw_gizmo.hpp>

#include "scene/entity.hpp"

#include <vector>
#include "octree_node.hpp"

BEGIN_XNOR_CORE

template <class T, class U>
concept ObjectBoundingT = Concepts::IsTSameAsU<ObjectBounding<T>,U>;

template <ObjectBoundingT T>
class OctreeIterator
{
    using Type = T;
    using PtrType = T*;
    using RefType = T&;

    OctreeIterator() = default;
    
    OctreeIterator(PtrType ptr) : m_Ptr(ptr)
    {
        
    } 

    ~OctreeIterator() = default;


    void operator++()
    {
       
    }

    void operator--()
    {
        
    }
    
    
    
private:
    PtrType m_Ptr;
};



template<class T>
class Octree
{
public:
    bool_t draw;
    
    void Update(std::vector<ObjectBounding<T>>& data);
    
    Octree() = default;

    ~Octree() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(Octree)
    
    void Draw();

    

private:
    void Clear();
    
    OctreeNode<T> m_MotherNode;
};

template <class T>
void Octree<T>::Update(std::vector<ObjectBounding<T>>& data)
{
    Clear();

    // Get the bounding box who contain all the data
    for (ObjectBounding<T>& element : data)
    {
        m_MotherNode.GetBound().Encapsulate(element.bound);
    }

    // normalize the mother box with his max to be a cube
    const Vector3 previousSize = m_MotherNode.GetBound().GetSize();
    const float_t maxSize = std::max( { previousSize.x , previousSize.y ,previousSize.z });
    const Vector3 size = Vector3(maxSize) * 0.5f;
    m_MotherNode.GetBound().SetMinMax(m_MotherNode.GetBound().center - size,m_MotherNode.GetBound().center + size);
    

    for (ObjectBounding<T>& element : data)
    {
        m_MotherNode.AddObject(element);
    }
}

template <class T>
void Octree<T>::Draw()
{
    if (draw)
        m_MotherNode.Draw();
}



template <class T>
void Octree<T>::Clear()
{
    m_MotherNode.Clear();
}


END_XNOR_CORE
