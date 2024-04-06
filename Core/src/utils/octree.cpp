#include "utils/octree.hpp"

#include "scene/component/mesh_renderer.hpp"

using namespace XnorCore;


Octree::Octree(const List<Entity*>& list , const float_t minNodeSize) : m_MinSize(minNodeSize)
{
    Compute(list);
}

void Octree::Draw()
{
    if (draw)
    rootNode.DrawGizmo();
}

void Octree::Compute(const List<Entity*>& list)
{
    if (!list.IsValid())
        return;
    
    Bound mainBound;
    
    for (uint32_t i = 0; i < list.GetSize(); i++)
    {
        const MeshRenderer* meshRenderer = list[i]->GetComponent<MeshRenderer>();
        if (meshRenderer == nullptr)
            continue;

        if (!meshRenderer->model.IsValid())
            continue;
            
        Bound currentObjectBounb = meshRenderer->model->GetAabb().GetAabbFromTransform(meshRenderer->entity->transform);
        mainBound.Encapsulate(currentObjectBounb);
    }

    rootNode = OctreeNode(mainBound, m_MinSize);

}
