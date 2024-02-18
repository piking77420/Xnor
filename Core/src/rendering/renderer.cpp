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

	m_Rhi.PrepareUniform();
}

void Renderer::RenderScene(const Scene& scene, const RendererContext& rendererContext) const
{
	/*
	if (!rendererContext.IsValid())
		return;//throw std::runtime_error("renderer Context is not valid");
	*/

	m_Rhi.SetClearColor(clearColor);
	m_Rhi.ClearColorAndDepth();

	basicShader->Use();

	CameraUniformData cam;
	rendererContext.camera->GetView(&cam.view);
	rendererContext.camera->GetProjection(&cam.projection);
	cam.cameraPos = { 1,0,0 };
	m_Rhi.UpdateCameraUniform(cam);


	std::vector<const MeshRenderer*> meshrenderer;
	scene.GetAllComponentOfType<MeshRenderer>(&meshrenderer);


	ModelUniformData modelData;
	modelData.model = Matrix::Identity();

	m_Rhi.UpdateModelUniform(modelData);
	
	RHI::SetPolyGoneMode(FRONT_AND_BACK,FILL);
	RHI::DrawModel(model->GetID());

	basicShader->UnUse();

}

