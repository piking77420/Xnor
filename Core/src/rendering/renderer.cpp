#include "rendering/renderer.hpp"

#include <glad/glad.h>
#include "scene/component/mesh_renderer.hpp"

using namespace XnorCore;

Renderer::Renderer()
{
	gladLoadGL();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void Renderer::SetClearColor(const Vector4& color) const
{
	glClearColor(color.x, color.y, color.z, color.w);
}

void Renderer::ClearColorAndDepth()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::RenderScene(const Scene& scene, RendererContext& rendererContext) const
{
	if (!rendererContext.IsValid())
		return;//throw std::runtime_error("renderer Context is not valid");

	
	std::vector<const MeshRenderer*> meshrenderer;
	scene.GetAllComponentOfType<MeshRenderer>(&meshrenderer);

}

