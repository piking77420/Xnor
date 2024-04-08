#pragma once
#include "octree_node.hpp"
#include "scene/entity.hpp"

BEGIN_XNOR_CORE

template<class T>
class Octree
{
public:
    OctreeNode<T> rootNode;

    bool_t draw = false;

    bool_t drawWithChild = false;

    DEFAULT_COPY_MOVE_OPERATIONS(Octree)
    
    Octree() = default;
    
    ~Octree() = default;

    void Draw();
    
    void Compute(std::vector<OctreeNodeData<T>>& data, float_t minNodeSize);

private:
    float_t m_MinSize = 1.f;

    void ComputeRootBound(std::vector<OctreeNodeData<T>>& data);
};



template <class T>
void Octree<T>::Draw()
{
    if (drawWithChild)
    {
        rootNode.DrawGizmoWithChild();
        return;
    }
    
    if (draw)
    {
        rootNode.DrawGizmo();
    }
}

template <class T>
void Octree<T>::Compute(std::vector<OctreeNodeData<T>>& data, float_t newminNodeSize)
{
    m_MinSize = newminNodeSize;

    if (data.empty())
        return;
        
    ComputeRootBound(data);

    for (size_t i = 0; i < data.size(); i++)
    {
        rootNode.AddObject(data[i]);
    }
}

template <class T>
void Octree<T>::ComputeRootBound(std::vector<OctreeNodeData<T>>& data)
{
    Bound mainBound;
    
    for (size_t i = 0; i < data.size(); i++)
    {
       mainBound.Encapsulate(data[i].bound);
    }

    const Vector3 previousSize = mainBound.GetSize();
    const float_t maxSize = std::max( { previousSize.x , previousSize.y ,previousSize.z });
    const Vector3 size = Vector3(maxSize) * 0.5f;
    mainBound.SetMinMax(mainBound.center - size,mainBound.center + size);
    
    OctreeNodeData<T> rootNodeData;
    rootNodeData.bound = mainBound;

    rootNode.Update(rootNodeData);
    rootNode.minNodeSize = m_MinSize;

}

END_XNOR_CORE
