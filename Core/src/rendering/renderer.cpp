#include "rendering/renderer.hpp"

#include "resource/resource_manager.hpp"
#include "scene/component/mesh_renderer.hpp"

using namespace XnorCore;

Renderer::Renderer() : clearColor(0.5f)
{
	m_Rhi.SetClearColor(clearColor);

	vertexPath = FileManager::Load("assets/shaders/vertex.vert");
	fragmentPath = FileManager::Load("assets/shaders/fragment.frag");
	
	basicShader = new Shader();
	basicShader->Load(*vertexPath, *fragmentPath);

	const Pointer<File> modelFile = FileManager::Load("assets/models/cube.obj");
	model = ResourceManager::CreateAndLoad<Model>(modelFile);
}

void Renderer::RenderScene(const Scene& scene, const RendererContext& rendererContext) const
{
	m_Rhi.SetClearColor(clearColor);
	m_Rhi.ClearColorAndDepth();
	
	/*
	if (!rendererContext.IsValid())
		return;//throw std::runtime_error("renderer Context is not valid");
	*/
	std::vector<const MeshRenderer*> meshrenderer;
	scene.GetAllComponentOfType<MeshRenderer>(&meshrenderer);

	basicShader->Use();
	
	RHI::SetPolyGoneMode(FRONT_AND_BACK,FILL);
	RHI::DrawModel(model->GetID());

	basicShader->UnUse();

}

