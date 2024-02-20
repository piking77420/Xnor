#pragma once

#include <vector>

#include "definitions.hpp"
#include "window.hpp"
#include "applications/application.hpp"
#include "windows/inspector.hpp"
#include "windows/ui_window.hpp"

BEGIN_XNOR_EDITOR

class Editor : public XnorCore::Application
{
public:
	explicit Editor();

	~Editor() override;
	
	void Update() override;
	
	Inspector* GetInspector();

private:
	std::vector<UiWindow*> m_UiWindows;

	void BeginFrame();

	void EndFrame();

	void WorldBehaviours();

private:
	void CreateDefaultWindows();
	
	void BeginDockSpace() const;

	void EndDockSpace() const;
		
	void SetupImGuiStyle() const;
};

END_XNOR_EDITOR
