#include "utils/octree.hpp"

#include "scene/component/mesh_renderer.hpp"

using namespace XnorCore;

void Octree::Draw()
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

void Octree::Compute(const List<Entity*>& list, float_t minNodeSize)
{
    m_MinSize = minNodeSize;
    
    if (!list.IsValid())
        return;
    
    ComputeRootBound(list);

    for (size_t i = 0; i < list.GetSize(); i++)
    {
        const MeshRenderer* meshRenderer = list[i]->GetComponent<MeshRenderer>();
        if (meshRenderer == nullptr)
            continue;

        if (!meshRenderer->model.IsValid())
            continue;

        const Bound currentObjectBounb = Bound::GetAabbFromTransform(meshRenderer->model->GetAabb(),meshRenderer->entity->transform);
        rootNode.AddObject(currentObjectBounb);
    }
}


void Octree::ComputeRootBound(const List<Entity*>& list)
{
    Bound mainBound;

    for (size_t i = 0; i < list.GetSize(); i++)
    {
        const MeshRenderer* meshRenderer = list[i]->GetComponent<MeshRenderer>();
        if (meshRenderer == nullptr)
            continue;

        if (!meshRenderer->model.IsValid())
            continue;
    
        const Bound currentObjectBounb = Bound::GetAabbFromTransform(meshRenderer->model->GetAabb(),meshRenderer->entity->transform);
        mainBound.Encapsulate(currentObjectBounb);
    }

    const Vector3 previousSize = mainBound.GetSize();
    const float_t maxSize = std::max( { previousSize.x , previousSize.y ,previousSize.z });
    const Vector3 size = Vector3(maxSize) * 0.5f;
    mainBound.SetMinMax(mainBound.center - size, mainBound.center + size);
    
    rootNode = OctreeNode(mainBound, m_MinSize);
}

