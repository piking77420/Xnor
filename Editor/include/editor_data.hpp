#pragma once

#include "definitions.hpp"
#include "user_input.hpp"
#include "scene/entity.hpp"

BEGIN_XNOR_EDITOR

struct EditorData
{
    XnorCore::Entity* selectedEntity;
    UserInput userInput;
};

END_XNOR_EDITOR
