#include "imgui/imgui.h"
#include "rendering/renderer.hpp"
#include "scene/scene.hpp"
#include "utils/logger.hpp"
#include "editor.hpp"
#include "window.hpp"
#include "file/file_manager.hpp"
#include "resource/resource_manager.hpp"
#include "resource/texture.hpp"
#include "utils/utils.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
	using namespace XnorCore;
	using namespace XnorEditor;
	
	Logger::OpenDefaultFile();

	Window window;
	Editor editor(window);
	Renderer renderer;
	
	Scene::CreateBasicScene();

	FrameBuffer* renderBuffer = new FrameBuffer();
	Texture* mainRenderTexture = new Texture(Attachements::COLOR, renderBuffer->GetSize());
	
	std::vector<RenderTarget> renderTargets(1);
	renderTargets[0].texture = mainRenderTexture;
	renderBuffer->AttachColorAttachement(renderTargets);
	
	Camera cam;
	cam.pos = { 0, 0, -5 };

	while (!window.ShouldClose())
	{
		window.PollEvents();
		editor.BeginFrame();

		ImGui::Begin("Renderer Settings");
		
		if (ImGui::Button("Recompile Shader"))
			renderer.CompileShader();
		
		ImGui::End();
		
		editor.Update();
		
		RendererContext context
		{
			.camera = &cam,
			.framebuffer = renderBuffer
		};
		renderer.RenderScene(*Scene::scene, context);
		
		ImGui::Begin("Scene");
		ImGui::Image(Utils::IntToPointer<ImTextureID>(mainRenderTexture->GetId()), ImGui::GetContentRegionAvail());
		ImGui::End();
		
		editor.EndFrame();
		window.SwapBuffers();
	}

	ResourceManager::DeleteAll();

	delete mainRenderTexture;
	delete renderBuffer;
	delete Scene::scene;

	FileManager::UnloadAll();

	Logger::Stop();
	
	return 0;
}
