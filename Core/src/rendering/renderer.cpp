#include "rendering/renderer.hpp"

#include "GLFW/glfw3.h"
#include "resource/resource_manager.hpp"
#include "scene/component/mesh_renderer.hpp"

using namespace XnorCore;

Renderer::Renderer() : clearColor(0.5f)
{
	
	m_Rhi.SetClearColor(clearColor);

	m_VertexPath = FileManager::Load("assets/shaders/vertex.vert");
	m_FragmentPath = FileManager::Load("assets/shaders/fragment.frag");
	m_DiamondPath = new File("assets/textures/viking_room.png");

	m_BasicShader = new Shader();
	CompileShader();
	
	const Pointer<File> modelFile = FileManager::Load("assets/models/viking_room.obj");
	m_Model = ResourceManager::CreateAndLoad<Model>(modelFile);
	m_Diamondtexture = new Texture();
	m_Diamondtexture->Load(*m_DiamondPath);
	m_Rhi.PrepareUniform();
	m_BasicShader->SetInt("diffuseTexture",0);
	
	
	renderBuffer = new FrameBuffer();
	TextureCreateInfo createInfo
	{
		nullptr,
		renderBuffer->GetSize().x,
		renderBuffer->GetSize().y,
		TextureFiltering::LINEAR,
		TextureWrapping::NONE,
		TextureFormat::RGB,
		TextureInternalFormat::RGB_8
	};


	
	mainRenderTexture = new Texture();
	mainRenderTexture->Load(createInfo);

	
	std::vector<RenderTarget> renderTargets(1);
	renderTargets[0].texture = mainRenderTexture;

	renderBuffer->CreateAttachement(renderTargets);
}

Renderer::~Renderer()
{
	delete m_BasicShader;
	delete m_Diamondtexture;
	delete m_DiamondPath;
	delete mainRenderTexture;

}

void Renderer::RenderScene(const Scene& scene, [[maybe_unused]] const RendererContext& rendererContext) const
{
	/*
	if (!rendererContext.IsValid())
		return;//throw std::runtime_error("renderer Context is not valid");
	*/
	m_Rhi.ClearColorAndDepth();

	renderBuffer->BindFrameBuffer();
	m_Rhi.ClearColorAndDepth();

	rendererContext.camera->pos = {0,0,-5};

	m_Rhi.SetClearColor(clearColor);
	m_Rhi.ClearColorAndDepth();

	m_BasicShader->Use();
	m_Diamondtexture->BindTexture(0);
	m_BasicShader->SetVec3("color",{clearColor.y,clearColor.x,clearColor.z});
	CameraUniformData cam;
	cam.cameraPos = rendererContext.camera->pos;
	rendererContext.camera->GetView(&cam.view);
	rendererContext.camera->GetProjection(&cam.projection);
	m_Rhi.UpdateCameraUniform(cam);


	std::vector<const MeshRenderer*> meshrenderer;
	scene.GetAllComponentOfType<MeshRenderer>(&meshrenderer);

	float time = glfwGetTime();

	ModelUniformData modelData;
	modelData.model = Matrix::Trs(0,vec3(0,time,time),1);

	m_Rhi.UpdateModelUniform(modelData);
	
	RHI::SetPolyGoneMode(FRONT_AND_BACK,FILL);
	RHI::DrawModel(m_Model->GetId());

	m_BasicShader->UnUse();
	renderBuffer->UnBindFrameBuffer();
}

void Renderer::CompileShader()
{
	if(m_BasicShader->IsLoaded())
	{
		m_BasicShader->Recompile(*m_VertexPath, *m_FragmentPath);
	}
	else
	{
		m_BasicShader->Load(*m_VertexPath, *m_FragmentPath);

	}


}

