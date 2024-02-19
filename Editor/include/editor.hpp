#pragma once

#include <vector>

#include "definitions.hpp"
#include "window.hpp"
#include "windows/ui_window.hpp"

BEGIN_XNOR_EDITOR

class Editor
{
public:
	explicit Editor(XnorCore::Window& window);

	~Editor();

	void BeginFrame();

	void Update();

	void EndFrame();

private:
	XnorCore::Window* m_Window;

	std::vector<UiWindow*> m_UiWindows;

	void CreateDefaultWindows();
	
	void BeginDockSpace() const;

	void EndDockSpace() const;
		
	void SetupImGuiStyle() const;
};

END_XNOR_EDITOR
