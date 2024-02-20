#pragma once

#include "windows/ui_window.hpp"
#include "definitions.hpp"

#include "scene/entity.hpp"

BEGIN_XNOR_EDITOR

class SceneGraph : public UiWindow
{
public:
    using UiWindow::UiWindow;
    
    void Display() override;

private:
    void DisplayEntity(XnorCore::Entity* entity);
};

END_XNOR_EDITOR
