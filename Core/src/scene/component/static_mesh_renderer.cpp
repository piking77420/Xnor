#include "scene/component/static_mesh_renderer.hpp"

using namespace  XnorCore;

void StaticMeshRenderer::GetAabb(Bound* const bound) const
{
    *bound = {};
    
    for (size_t i = 0; i < mesh->models.GetSize(); i++)
    {
        const Pointer<Model>& model = mesh->models[i];
        
       bound->Encapsulate(model->GetAabb());
    }
    
    *bound = Bound::GetAabbFromTransform(*bound, GetTransform());
}
