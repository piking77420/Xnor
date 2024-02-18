#pragma once

#include "definitions.hpp"
#include "entity_test.hpp"
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
	EntityDamage m_EntityTest;
};

END_XNOR_EDITOR
