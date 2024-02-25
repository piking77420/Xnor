#pragma once

#include <vector>

#include "definitions.hpp"
#include "editor_data.hpp"
#include "application.hpp"

BEGIN_XNOR_EDITOR

class UiWindow;
class RenderWindow;
class UserInput;

class Editor : public XnorCore::Application
{
public:
	explicit Editor();

	~Editor() override;

	DEFAULT_COPY_MOVE_OPERATIONS_NO_ENGINE(Editor)
	
	void Update() override;

	EditorData data;
	
private:
	XnorCore::RendererContext m_GameRenderContext;
	XnorCore::RendererContext m_EditorRenderContext;

	std::vector<UiWindow*> m_UiWindows;

	void UpdateWindow();
	
	void BeginFrame();

	void EndFrame();

	void WorldBehaviours();

	void CreateDefaultWindows();
	
	void BeginDockSpace() const;

	void EndDockSpace() const;
		
	void SetupImGuiStyle() const;

	void CreateTestScene();
};

END_XNOR_EDITOR
