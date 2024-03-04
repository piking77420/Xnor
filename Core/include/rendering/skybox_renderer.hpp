#pragma once

#include "core.hpp"
#include "resource/cubemap.hpp"
#include "resource/model.hpp"
#include "resource/shader.hpp"
#include "utils/pointer.hpp"
#include "world/skybox.hpp"

BEGIN_XNOR_CORE

class SkyboxRenderer
{
public:
    XNOR_ENGINE SkyboxRenderer() = default;

    XNOR_ENGINE ~SkyboxRenderer() = default;

    DEFAULT_COPY_MOVE_OPERATIONS(SkyboxRenderer)

    XNOR_ENGINE void InitializeResources();

    XNOR_ENGINE void DrawSkymap(const Pointer<Model>& cubeModel,const Skybox& skybox) const;
  
private:
    Pointer<Shader> m_SkyboxDrawerShader;
};

END_XNOR_CORE
