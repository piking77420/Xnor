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

    XnorCore::Entity* m_EntityToDelete;
    XnorCore::Entity* m_EntityToRename;
};

END_XNOR_EDITOR
