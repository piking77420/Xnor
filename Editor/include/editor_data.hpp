#pragma once

#include "definitions.hpp"
#include "input/input.hpp"
#include "rendering/camera.hpp"
#include "scene/entity.hpp"

BEGIN_XNOR_EDITOR

struct EditorData
{
    XnorCore::Entity* selectedEntity = nullptr;
    
    XnorCore::Camera gameCam;
    XnorCore::Camera editorCam;

    XnorCore::Pointer<XnorCore::File> currentScene;
};

END_XNOR_EDITOR
