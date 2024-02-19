#include "rendering/renderer.hpp"

#include "GLFW/glfw3.h"
#include "resource/resource_manager.hpp"
#include "scene/component/mesh_renderer.hpp"

using namespace XnorCore;

Renderer::Renderer()
	: clearColor(0.5f)
{
	m_Rhi.SetClearColor(clearColor);

	m_VertexPath = FileManager::Load("assets/shaders/vertex.vert");
	m_FragmentPath = FileManager::Load("assets/shaders/fragment.frag");
	m_DiamondPath = FileManager::Load("assets/textures/viking_room.png");

	m_BasicShader = ResourceManager::Add<Shader>("assets/shaders/shader");
	m_BasicShader->Load(*m_VertexPath, *m_FragmentPath);
	CompileShader();
	
	const Pointer<File> modelFile = FileManager::Load("assets/models/viking_room.obj");
	m_Model = ResourceManager::Load<Model>(modelFile);
	m_Diamondtexture = ResourceManager::Load<Texture>(m_DiamondPath);
	m_Rhi.PrepareUniform();
	m_BasicShader->SetInt("diffuseTexture", 0);
}

void Renderer::RenderScene(const Scene& scene, [[maybe_unused]] const RendererContext& rendererContext) const
{
	m_Rhi.ClearColorAndDepth();

	if (rendererContext.framebuffer != nullptr)
		rendererContext.framebuffer->BindFrameBuffer();
	
	m_Rhi.ClearColorAndDepth();

	m_Rhi.SetClearColor(clearColor);
	m_Rhi.ClearColorAndDepth();

	m_BasicShader->Use();
	m_Diamondtexture->BindTexture(0);
	m_BasicShader->SetVec3("color", {clearColor.y, clearColor.x, clearColor.z});
	CameraUniformData cam;
	cam.cameraPos = rendererContext.camera->pos;
	rendererContext.camera->GetView(&cam.view);
	rendererContext.camera->GetProjection(&cam.projection);
	m_Rhi.UpdateCameraUniform(cam);

	std::vector<const MeshRenderer*> meshrenderer;
	scene.GetAllComponentOfType<MeshRenderer>(&meshrenderer);

	const float_t time = static_cast<float_t>(glfwGetTime());

	ModelUniformData modelData;
	modelData.model = Matrix::Trs(Vector3::Zero(), Vector3(0, time, time), Vector3(1.f));

	m_Rhi.UpdateModelUniform(modelData);
	
	RHI::SetPolyGoneMode(PolyGoneFace::FRONT_AND_BACK, PolyGoneMode::FILL);
	RHI::DrawModel(m_Model->GetId());

	m_BasicShader->UnUse();
	
	if (rendererContext.framebuffer != nullptr)
		rendererContext.framebuffer->UnBindFrameBuffer();
}

void Renderer::CompileShader()
{
	if (m_BasicShader->IsLoaded())
	{
		m_BasicShader->Recompile(*m_VertexPath, *m_FragmentPath);
	}
	else
	{
		m_BasicShader->Load(*m_VertexPath, *m_FragmentPath);
	}
}
