#pragma once
#include "core.hpp"
#include "rendering/material.hpp"
#include "resource/model.hpp"
#include "resource/texture.hpp"
#include "scene/component.hpp"

BEGIN_XNOR_CORE

class MeshRenderer : public Component
{
    REFLECTABLE_IMPL(MeshRenderer)
    
public:
    XNOR_ENGINE MeshRenderer();
    Pointer<Model> model;
    Material material;
    bool drawModelAabb = false;
};

END_XNOR_CORE

REFL_AUTO(
    type(XnorCore::MeshRenderer, bases<XnorCore::Component>),
    field(model),
    field(material),
    field(drawModelAabb)
);
