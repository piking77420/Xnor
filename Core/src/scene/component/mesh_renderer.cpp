#include "scene/component/mesh_renderer.hpp"

using namespace  XnorCore;

void MeshRenderer::GetAabb(Bound* bound) const
{
    if (!model.IsValid())
        return;
            
    *bound = Bound::GetAabbFromTransform(model->GetAabb(), GetTransform());
}
