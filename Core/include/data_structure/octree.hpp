#pragma once


#include <rendering/draw_gizmo.hpp>

#include "scene/entity.hpp"

#include <vector>
#include "octree_node.hpp"

BEGIN_XNOR_CORE



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
    
    OctreeNode<T> motherNode;
};

template <class T>
void Octree<T>::Update(std::vector<ObjectBounding<T>>& data)
{
    Clear();

    // Get the bounding box who contain all the data
    for (ObjectBounding<T>& element : data)
    {
        motherNode.GetBound().Encapsulate(element.bound);
    }

    // normalize the mother box with his max to be a cube
    const Vector3 previousSize = motherNode.GetBound().GetSize();
    const float_t maxSize = std::max( { previousSize.x , previousSize.y ,previousSize.z });
    const Vector3 size = Vector3(maxSize) * 0.5f;
    motherNode.GetBound().SetMinMax(motherNode.GetBound().center - size,motherNode.GetBound().center + size);
    

    for (ObjectBounding<T>& element : data)
    {
        motherNode.AddObject(element);
    }
}

template <class T>
void Octree<T>::Draw()
{
    if (draw)
        motherNode.Draw();
}



template <class T>
void Octree<T>::Clear()
{
    motherNode.Clear();
}


END_XNOR_CORE
