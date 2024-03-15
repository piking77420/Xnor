#pragma once

#include <array>
#include <string>

#include "core.hpp"
#include "rendering/render_systems/equirectangular_to_cubemap.hpp"
#include "resource/cubemap.hpp"
#include "resource/shader.hpp"
#include "resource/texture.hpp"

/// @file skybox.hpp
/// @brief Defines the XnorCore::Skybox class.

BEGIN_XNOR_CORE


enum class SkyBoxType
{
    CubeMap,
    Hdr,
    Procedural
};

/// @brief A wrapper for a skybox
class XNOR_ENGINE Skybox
{
private:
    static inline Vector2i m_EnvironementCubeMapSize { 512 , 512 };
public:

    Skybox() = default;

    ~Skybox() = default;


    void Initialize();
    
    /// @brief Loads a cube map using an array of 6 different file paths
    /// @param cubeMapFiles Array of 6 texture file paths
    void LoadCubeMap(const std::array<std::string, 6>& cubeMapFiles);
    
    /// @brief take a hdr file in input and wrap it to cubeMap
    /// @param hdfFile hdr image
    void LoadFromHdrTexture(const Pointer<Texture>& hdfFile);

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

    EquirectangularToCubeMap m_EquirectangularToCubeMap;
};

END_XNOR_CORE
