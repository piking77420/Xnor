#pragma once

#include "definitions.hpp"
#include "window.hpp"

BEGIN_XNOR_EDITOR

class Editor
{
public:
	explicit Editor(const XnorCore::Window& window);

	~Editor();

	void BeginFrame();

	void EndFrame(const XnorCore::Window& window);
};

END_XNOR_EDITOR
