#include "scene/component/mesh_renderer.hpp"

using namespace  XnorCore;

void MeshRenderer::GetAABB(Bound* bound) const
{
    if (!model.IsValid())
        return;
            
    *bound = Bound::GetAabbFromTransform(model->GetAabb(),GetTransform());
}
