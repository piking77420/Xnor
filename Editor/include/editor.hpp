#pragma once

#include "definitions.hpp"
#include "window.hpp"
#include "windows/inspector.hpp"

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
	Inspector m_Inspector;

	void BeginDockSpace() const;

	void EndDockSpace() const;
		
	void SetupImGuiStyle() const;
	
};

END_XNOR_EDITOR
