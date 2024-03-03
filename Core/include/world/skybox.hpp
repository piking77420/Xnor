#pragma once

#include <array>
#include <string>

#include "core.hpp"
#include "resource/cubemap.hpp"

BEGIN_XNOR_CORE
class XNOR_ENGINE Skybox
{
public:
    void LoadCubeMap(const std::array<std::string,6>& cubeMapFiles);

    [[nodiscard]]
    const Cubemap* GetCubeMap() const;

    [[nodiscard]]
    const Cubemap* GetCubeMap();
    
private:
    Cubemap* m_CubeMap = nullptr;
};

END_XNOR_CORE