#include "editor.hpp"

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <ImGui/imgui_internal.h>
#include <ImguiGizmo/ImGuizmo.h>

#include "file/file_manager.hpp"
#include "input/time.hpp"
#include "rendering/light/directional_light.hpp"
#include "rendering/light/point_light.hpp"
#include "resource/resource_manager.hpp"
#include "scene/component/mesh_renderer.hpp"
#include "scene/component/test_component.hpp"
#include "serialization/serializer.hpp"
#include "windows/content_browser.hpp"
#include "windows/editor_window.hpp"
#include "windows/header_window.hpp"
#include "windows/hierarchy.hpp"
#include "windows/inspector.hpp"
#include "windows/performance.hpp"
#include "windows/render_window.hpp"
#include "world/scene_graph.hpp"
#include "world/world.hpp"

using namespace XnorEditor;

Editor::Editor()
{
	const XnorCore::Pointer<XnorCore::File> logoFile = XnorCore::FileManager::Get("assets_internal/editor/logo.png");
	XnorCore::Pointer<XnorCore::Texture> logo = XnorCore::ResourceManager::Get<XnorCore::Texture>(logoFile);
	logo->loadData.desiredChannels = 4;
	logo->Reload(logoFile);
	XnorCore::Window::SetIcon(*logo);
	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	io.Fonts->AddFontDefault();

	constexpr const char* glslVersion = "#version 460";

	ImGui_ImplGlfw_InitForOpenGL(XnorCore::Window::GetHandle(), true);
	ImGui_ImplOpenGL3_Init(glslVersion);

	SetupImGuiStyle();
	CreateDefaultWindows();
}

Editor::~Editor()
{
	for (const UiWindow* w : m_UiWindows)
		delete w;
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyPlatformWindows();
	ImGui::DestroyContext();
}

void Editor::CreateDefaultWindows()
{
	m_UiWindows.push_back(new Performance(this, 50));
	m_UiWindows.push_back(new Inspector(this));
	m_UiWindows.push_back(new HeaderWindow(this));
	m_UiWindows.push_back(new Hierarchy(this));
	m_UiWindows.push_back(new ContentBrowser(this, XnorCore::FileManager::Get<XnorCore::Directory>("assets")));
	m_UiWindows.push_back(new RenderWindow(this));
	m_UiWindows.push_back(new EditorWindow(this));

	if (XnorCore::FileManager::Contains(SerializedScenePath))
		data.currentScene = XnorCore::FileManager::Get<XnorCore::File>(SerializedScenePath);
}

void Editor::BeginDockSpace() const
{
	static bool dockspaceOpen = true;
	static bool optFullscreenPersistant = true;
	const bool optFullscreen = optFullscreenPersistant;
	
	ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
	if (optFullscreen)
		dockspaceFlags |= ImGuiDockNodeFlags_PassthruCentralNode;
	
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (optFullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;
	}

	// Begin docking layout
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, windowFlags);
	
	if (optFullscreen)
		ImGui::PopStyleVar(3);

	const ImGuiID dockspaceId = ImGui::GetID("DockSpace");
	ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void Editor::EndDockSpace() const
{
	ImGui::End();
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void Editor::SetupImGuiStyle() const
{
	// XNOR Engine style from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();
	
	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6000000238418579f;
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(4.0f, 3.0f);
	style.FrameRounding = 0.0f;
	style.FrameBorderSize = 1.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 14.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 0.0f;
	style.TabRounding = 0.0f;
	style.TabBorderSize = 1.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
	
	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.729411780834198f, 0.7490196228027344f, 0.7372549176216125f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.08627451211214066f, 0.08627451211214066f, 0.08627451211214066f, 0.9399999976158142f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.9399999976158142f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 1.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.5400000214576721f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1764705926179886f, 0.1764705926179886f, 0.1764705926179886f, 0.4000000059604645f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 0.6700000166893005f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 0.6523605585098267f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 0.6700000166893005f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.01960784383118153f, 0.01960784383118153f, 0.01960784383118153f, 0.5299999713897705f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.407843142747879f, 0.407843142747879f, 0.407843142747879f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.0f, 0.3450980484485626f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.0f, 0.3433477878570557f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.3803921937942505f, 0.6888663768768311f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.0f, 0.0f, 0.0f, 0.5411764979362488f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1764705926179886f, 0.1764705926179886f, 0.1764705926179886f, 0.4000000059604645f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 0.6705882549285889f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.2705882489681244f, 0.2705882489681244f, 0.2705882489681244f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.3529411852359772f, 0.3529411852359772f, 0.3529411852359772f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.0f, 0.3433477878570557f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.3294117450714111f, 0.4387779831886292f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.0f, 0.3175966739654541f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.4862744808197021f, 0.636202871799469f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.4862744808197021f, 0.6626610159873962f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.2196078449487686f, 0.2196078449487686f, 0.2196078449487686f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.0f, 0.5113187432289124f, 0.8509804010391235f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.0f, 0.5543665885925293f, 0.8497853875160217f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.05157577246427536f, 0.2260260879993439f, 0.6008583307266235f, 0.9686274528503418f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.06229622289538383f, 0.3212751150131226f, 0.7639485001564026f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.6078431606292725f, 0.6078431606292725f, 0.6078431606292725f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.0f, 0.4239671230316162f, 0.8980392217636108f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.0f, 0.3218332231044769f, 0.721030056476593f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.3019607961177826f, 0.3019607961177826f, 0.3019607961177826f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2627451121807098f, 0.6352941393852234f, 0.8784313797950745f, 0.4377682209014893f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.1843137294054031f, 0.1843137294054031f, 0.4666666686534882f, 0.9647058844566345f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.407843142747879f, 0.407843142747879f, 0.407843142747879f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void Editor::CreateTestScene()
{
	using namespace XnorCore;
	
	// init Scene //
	Entity& ent1 = *World::scene.CreateEntity("VikingRoom");
	MeshRenderer* meshRenderer = ent1.AddComponent<MeshRenderer>();
	ent1.transform.SetPosition() = { 0.f, 3.f, 0.f };
	
	meshRenderer->model = ResourceManager::Get<Model>(FileManager::Get("assets/models/viking_room.obj"));
	Pointer<File>&& vikingRoomTexture = FileManager::Get("assets/textures/viking_room.png");
	meshRenderer->material.albedo = ResourceManager::Get<Texture>(vikingRoomTexture);
	meshRenderer->material.albedo->loadData.flipVertically = true;
	meshRenderer->material.albedo->Unload();
	meshRenderer->material.albedo->Load(vikingRoomTexture);
	meshRenderer->material.albedo->CreateInRhi();

	Entity& ent2 = *World::scene.CreateEntity("DirectionalLight");
	DirectionalLight* dirlight = ent2.AddComponent<DirectionalLight>();
	dirlight->intensity = 0.2f;
	dirlight->color = Colorf(1.f, 1.f, 1.f);

	Entity& pointLightentity = *World::scene.CreateEntity("PointLight");
	PointLight* pointLight = pointLightentity.AddComponent<PointLight>();
	pointLight->color = { 7.7f, 0.f, 3.f };
	pointLightentity.AddComponent<TestComponent>();
	pointLightentity.transform.SetPosition() = { 0.f, 2.f, -2.f };
	
	Entity& ent3 = *World::scene.CreateEntity("Plane");
	meshRenderer = ent3.AddComponent<MeshRenderer>();
	meshRenderer->model = ResourceManager::Get<Model>("assets/models/cube.obj");
	meshRenderer->material.albedo = ResourceManager::Get<Texture>("assets/textures/wood.jpg");
	ent3.transform.SetScale() = { 10.f, 0.1f, 10.f };
	ent3.transform.SetPosition() -= { 0.f, -0.2f, 0.f};
	
	Entity& ent4 = *World::scene.CreateEntity("CubeMinecraft");
	ent4.transform.SetPosition() = { 2.f, 0, 2.f};
	meshRenderer = ent4.AddComponent<MeshRenderer>();
	meshRenderer->model = ResourceManager::Get<Model>("assets/models/cube.obj");
	meshRenderer->material.albedo = ResourceManager::Get<Texture>("assets/textures/diamond_block.jpg");

	const std::array<std::string, 6> testCubeMap
	{
		"assets/skybox/right.jpg",
		"assets/skybox/left.jpg",
		"assets/skybox/top.jpg",
		"assets/skybox/bottom.jpg",
		"assets/skybox/front.jpg",
		"assets/skybox/back.jpg"
	};
	World::skybox.LoadCubeMap(testCubeMap);
}

void Editor::MenuBar() const
{
	if (ImGui::BeginMainMenuBar())
	{	
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				std::string path;
				if (data.currentScene == nullptr)
				{
					if (!std::filesystem::exists("assets/scenes"))
						std::filesystem::create_directories("assets/scenes");
					path = SerializedScenePath;
				}
				else
				{
					path = data.currentScene->GetPathString();
				}
				XnorCore::Serializer::StartSerialization(path);
				XnorCore::World::scene.Serialize();
				XnorCore::Serializer::EndSerialization();
			}
			
			ImGui::EndMenu();
		}
		
		ImGui::EndMainMenuBar();
	}
}

void Editor::UpdateWindow()
{
	for (UiWindow* w : m_UiWindows)
	{
		ImGui::Begin(w->GetName(),nullptr,w->windowFlags);
		w->FetchInfo();
		w->Display();
		ImGui::End();
	}
}

void Editor::BeginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
	
	BeginDockSpace();
	MenuBar();
}

void Editor::Update()
{
	using namespace XnorCore;

	renderer.PrepareRendering(Window::GetSize());
	CreateTestScene();

	Window::Show();
	
	while (!Window::ShouldClose())
	{
		Time::Update();
		Window::PollEvents();
		Input::HandleEvent();
		BeginFrame();
		CheckWindowResize();

		ImGui::Begin("Renderer Settings");
		if (ImGui::Button("Recompile Shader"))
			renderer.CompileShader();
		ImGui::End();

		UpdateWindow();
		WorldBehaviours();
	
		Input::Update();
		EndFrame();
		renderer.SwapBuffers();
	}
}

void Editor::CheckWindowResize()
{
	if (!XnorCore::Window::resizeFrameBuffer)
		return;

	const Vector2i newWindowSize = XnorCore::Window::GetSize();

	renderer.OnResize(newWindowSize);
	
	for (UiWindow* w : m_UiWindows)
	{
		w->OnWindowResize(newWindowSize);
	}
	
	XnorCore::Window::resizeFrameBuffer = false;
}

void Editor::EndFrame()
{
	EndDockSpace();
	
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	XnorCore::Window::MakeContextCurrent();
}

void Editor::WorldBehaviours()
{
	XnorCore::SceneGraph::Update(XnorCore::World::scene.GetEntities());
	
	if (XnorCore::World::isPlaying)
	{
		if (!XnorCore::World::hasStarted)
		{
			XnorCore::World::Begin();
			XnorCore::World::hasStarted = true;
		}

		XnorCore::World::Update();
	}
}
