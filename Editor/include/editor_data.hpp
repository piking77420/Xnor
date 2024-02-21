#pragma once

#include "definitions.hpp"
#include "scene/entity.hpp"

BEGIN_XNOR_EDITOR

struct EditorData
{
    XnorCore::Entity* selectedEntity;
    float_t cameraSpeed = 3;
};

END_XNOR_EDITOR
