#include "rendering/renderer.hpp"

#include "scene/component/mesh_renderer.hpp"

using namespace XnorCore;

Renderer::Renderer()
{
	m_Rhi.SetClearColor(clearColor);
}

void Renderer::RenderScene(const Scene& scene, const RendererContext& rendererContext) const
{
	m_Rhi.SetClearColor(clearColor);
	m_Rhi.ClearColorAndDepth();
	
	if (!rendererContext.IsValid())
		return;//throw std::runtime_error("renderer Context is not valid");
	
	std::vector<const MeshRenderer*> meshrenderer;
	scene.GetAllComponentOfType<MeshRenderer>(&meshrenderer);

	RHI::SetPolyGoneMode(FRONT_AND_BACK,FILL);
}

