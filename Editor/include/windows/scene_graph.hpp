#pragma once

#include "windows/ui_window.hpp"
#include "definitions.hpp"

#include "scene/entity.hpp"

class SceneGraph : public UiWindow
{
public:
    void Display() override;

private:
    void DisplayEntity(XnorCore::Entity* entity);
};
