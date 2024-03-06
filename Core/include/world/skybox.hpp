#pragma once

#include <array>
#include <string>

#include "core.hpp"
#include "resource/cubemap.hpp"

/// @file skybox.hpp
/// @brief Defines the Skybox class.

BEGIN_XNOR_CORE

/// @brief A wrapper for a skybox
class XNOR_ENGINE Skybox
{
public:
    /// @brief Loads a cube map using an array of 6 different file paths
    /// @param cubeMapFiles Array of 6 texture file paths
    void LoadCubeMap(const std::array<std::string, 6>& cubeMapFiles);

    /// @brief Gets the cubemap of the skybox
    /// @return Cubemap pointer
    [[nodiscard]]
    const Cubemap* GetCubeMap() const;

    /// @brief Gets the cubemap of the skybox
    /// @return Cubemap pointer
    [[nodiscard]]
    Cubemap* GetCubeMap();
    
private:
    Cubemap* m_CubeMap = nullptr;
};

END_XNOR_CORE
