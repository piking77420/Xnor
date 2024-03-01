#pragma once

#include "core.hpp"
#include "resource/cubemap.hpp"
#include "resource/model.hpp"
#include "resource/shader.hpp"
#include "utils/pointer.hpp"

BEGIN_XNOR_CORE
enum class SkyMode
{
    CubeMap,
    Procedurral
};

class SkyboxRenderer
{
public:
    
    
    XNOR_ENGINE SkyboxRenderer();

    XNOR_ENGINE ~SkyboxRenderer();

    XNOR_ENGINE void InitializeResources();

    XNOR_ENGINE void DrawSkymap(const Pointer<Model>& cubeModel) const;
  
private:

    Cubemap* m_Cubemap;
    Pointer<Shader> m_SkyboxDrawerShader;
};

END_XNOR_CORE
