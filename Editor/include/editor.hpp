#pragma once

#include "definitions.hpp"
#include "window.hpp"

BEGIN_XNOR_EDITOR

class Editor
{
public:
	explicit Editor(XnorCore::Window& window);

	~Editor();

	void BeginFrame();

	void EndFrame();

private:
	XnorCore::Window* m_Window;
};

END_XNOR_EDITOR
