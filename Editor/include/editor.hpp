#pragma once

#include <vector>

#include "definitions.hpp"
#include "editor_data.hpp"
#include "user_input.hpp"
#include "applications/application.hpp"

BEGIN_XNOR_EDITOR
class UiWindow;
class RenderWindow;
class UserInput;

class Editor : public XnorCore::Application
{
public:
	explicit Editor();

	DEFAULT_COPY_MOVE_OPERATIONS_NO_ENGINE(Editor)

	~Editor() override;
	
	void Update() override;

	EditorData data;
	
private:
	
	XnorCore::RendererContext m_GameRenderContext;

	std::vector<UiWindow*> m_UiWindows;

	void BeginFrame();

	void EndFrame();

	void WorldBehaviours();

	void CreateDefaultWindows();
	
	void BeginDockSpace() const;

	void EndDockSpace() const;
		
	void SetupImGuiStyle() const;
};

END_XNOR_EDITOR
