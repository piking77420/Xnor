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
    
    Octree(std::vector<ObjectBounding<T>>& data)
    {
        for (ObjectBounding<T>& element : data)
        {
            motherNode.GetBound().Encapsulate(element.bound);
        }

        for (ObjectBounding<T>& element : data)
        {
            motherNode.AddObject(element);
        }
    }
    
    Octree() = default;

    void Draw();
    
private:
    OctreeNode<T> motherNode;
};

template <class T>
void Octree<T>::Draw()
{
    if (draw)
        motherNode.Draw();
}


END_XNOR_CORE
