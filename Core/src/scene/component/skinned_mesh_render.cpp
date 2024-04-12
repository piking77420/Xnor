#include "scene/component/skinned_mesh_render.hpp"

using namespace XnorCore;

void SkinnedMeshRender::Update()
{
    mesh->animations[0]->Update();
}
