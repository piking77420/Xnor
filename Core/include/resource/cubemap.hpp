#pragma once

#include <array>
#include <Maths/vector2i.hpp>

#include "core.hpp"
#include "resource.hpp"
#include "rendering/rhi_typedef.hpp"

/// @file cubemap.hpp
/// @brief Defines the XnorCore::Cubemap class

BEGIN_XNOR_CORE

/// @brief Represents an array of 6 @ref Texture "Textures"
class Cubemap : public Resource
{
public:
    /// @brief Creates a cubemap using 6 provided texture file names
    /// @param cubeMapsTextures Texture file names
    XNOR_ENGINE explicit Cubemap(const std::array<std::string, 6>& cubeMapsTextures);

    /// @brief Creates a cubemap using texture creation info
    /// @param textureCreateInfo Texture creation info
    XNOR_ENGINE explicit Cubemap(const TextureCreateInfo& textureCreateInfo);

    XNOR_ENGINE ~Cubemap() override;
    
    /// @brief Creates the Cubemap in the Rhi
    XNOR_ENGINE void CreateInRhi() override;

    /// @brief Destroys the Cubemap in the Rhi
    XNOR_ENGINE void DestroyInRhi() override;

    /// @brief Unloads the Cubemap
    XNOR_ENGINE void Unload() override;

    /// @brief Binds the Cubemap
    XNOR_ENGINE void BindTexture(uint32_t unit) const;

    /// @brief Unbinds the Cubemap
    XNOR_ENGINE void UnBindTexture(uint32_t unit) const;

    /// @brief Gets the CubeMap id
    /// @return CubeMap id
    [[nodiscard]]
    XNOR_ENGINE uint32_t GetId() const;

    /// @brief Gets the CubeMap Size
    /// @return CubeMap size
    [[nodiscard]]
    XNOR_ENGINE Vector2i GetSize() const;
    
private:
    uint32_t m_Id = 0;
    std::vector<void*> m_Images =
    {
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    };
    
    Vector2i m_CubemapSize;
    int32_t m_DataChannels = 0;

    TextureFiltering::TextureFiltering m_TextureFiltering = TextureFiltering::Linear;
    TextureWrapping::TextureWrapping m_TextureWrapping = TextureWrapping::ClampToEdge;
    TextureInternalFormat::TextureInternalFormat m_TextureInternalFormat = TextureInternalFormat::Rgba8;
    TextureFormat::TextureFormat m_TextureFormat = TextureFormat::Rgb;
    
};

END_XNOR_CORE
