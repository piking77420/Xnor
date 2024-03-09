#pragma once

#include <vector>

#include "application.hpp"
#include "definitions.hpp"
#include "editor_data.hpp"

BEGIN_XNOR_EDITOR

class UiWindow;
class RenderWindow;
class UserInput;

class Editor : public XnorCore::Application
{
public:
	void CheckWindowResize() override;

	EditorData data;

	explicit Editor();

	~Editor() override;

	DEFAULT_COPY_MOVE_OPERATIONS(Editor)
	
	void Update() override;

private:
	static constexpr const char_t* SerializedScenePath = "assets/scenes/basic_scene.scene.xml";
	
	XnorCore::Viewport m_GameRenderContext;
	XnorCore::Viewport m_EditorRenderContext;

	std::vector<UiWindow*> m_UiWindows;

	void UpdateWindow();

	void OnRenderingWindow();
	
	void BeginFrame();

	void EndFrame();
	
	void WorldBehaviours();

	void CreateDefaultWindows();
	
	void BeginDockSpace() const;

	void EndDockSpace() const;
		
	void SetupImGuiStyle() const;

	void CreateTestScene();
	
	void MenuBar() const;
};

END_XNOR_EDITOR
