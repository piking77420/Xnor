#include "scene/component/mesh_renderer.hpp"

using namespace  XnorCore;

void MeshRenderer::GetAABB(Bound* bound) const
{
    *bound = Bound();
    
    for (size_t i = 0; i < mesh->models.GetSize(); i++)
    {
        const Pointer<Model>& model = mesh->models[i];
        
       bound->Encapsulate(model->GetAabb());
    }
    
    *bound = Bound::GetAabbFromTransform(*bound,GetTransform());
}
