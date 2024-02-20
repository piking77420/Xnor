#pragma once
#include "core.hpp"
#include "resource/model.hpp"
#include "resource/texture.hpp"
#include "scene/component.hpp"

BEGIN_XNOR_CORE

class MeshRenderer : public Component
{
public:
    Pointer<Model> model;
    Pointer<Texture> texture;
};

END_XNOR_CORE