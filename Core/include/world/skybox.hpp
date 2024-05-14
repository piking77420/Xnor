#pragma once

#include <array>
#include <string>

#include "core.hpp"
#include "rendering/render_systems/skybox_parser.hpp"
#include "resource/cubemap.hpp"
#include "resource/shader.hpp"
#include "resource/texture.hpp"

/// @file skybox.hpp
/// @brief Defines the XnorCore::Skybox class.

BEGIN_XNOR_CORE

/// @brief A wrapper for a skybox
class Skybox
{
    constexpr static Vector2i EnvironementCubeMapSize { 512, 512 };
    constexpr static Vector2i IradianceCubeSize { 32, 32 };
    constexpr static Vector2i PrefilterMapSize { 128, 128 };
    constexpr static uint32_t MaxMinMapLevel = 5;

public:
    XNOR_ENGINE Skybox() = default;
    XNOR_ENGINE ~Skybox();

    DEFAULT_COPY_MOVE_OPERATIONS(Skybox)

    /// @brief Initializes the skybox
    XNOR_ENGINE void Initialize();
    
    /// @brief Loads a cube map using an array of 6 different file paths
    /// @param cubeMapFiles Array of 6 texture file paths
    XNOR_ENGINE void LoadCubeMap(const std::array<std::string, 6>& cubeMapFiles);
    
    /// @brief take a hdr file in input and wrap it to cubeMap
    /// @param hdfFile hdr image
    XNOR_ENGINE void LoadFromHdrTexture(const Pointer<Texture>& hdfFile) const;

    /// @brief Bind All Texture For Rendering
    XNOR_ENGINE void BindDesriptorSet() const;
    
    /// @brief UnBind All Texture For Rendering
    XNOR_ENGINE void UnbindDesriptorSet() const;

    /// @brief Gets the skybox albedo color map
    /// @returns Cubemap albedo
    [[nodiscard]]
    XNOR_ENGINE const Cubemap* GetSkyboxAlbedoColor() const;

private:
    /// @brief Irradiance map
    Cubemap* m_IrradianceMap = nullptr;

    /// @brief Cube map
    Cubemap* m_CubeMap = nullptr;

    /// @brief Pre-filter map
    Cubemap* m_PrefilterMap = nullptr;

    /// @brief Pre-compute brdf texture
    Texture* m_PrecomputeBrdfTexture = nullptr;
};

END_XNOR_CORE
