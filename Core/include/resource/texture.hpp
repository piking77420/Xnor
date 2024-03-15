#pragma once

#include <array>

#include <Maths/vector2i.hpp>

#include "core.hpp"
#include "resource.hpp"
#include "rendering/rhi_typedef.hpp"

/// @file texture.hpp
/// @brief Defines the XnorCore::Texture class

BEGIN_XNOR_CORE

/// @brief Represents an image in memory.
class Texture : public Resource
{
public:
    /// @brief Options for loading the data of a Texture.
    ///
    /// @see Texture::loadData
    struct LoadOptions
    {
        int32_t desiredChannels = 0;
        bool_t flipVertically = false;
    };

    /// @brief Allowed extensions for texture files
    XNOR_ENGINE static inline constexpr std::array<const char_t*, 11> FileExtensions
    {
        ".jpg",
        ".jpeg",
        ".png",
        ".bmp",
        ".hdr",
        ".psd",
        ".tga",
        ".gif",
        ".pic",
        ".pgm",
        ".ppm"
    };

    /// @brief Options to use when loading this Texture.
    ///
    /// This doesn't affect an already-loaded Texture. If necessary, change these settings and then call Texture::Reload for the changes to take effect.
    LoadOptions loadData;
    
    // Same constructor from base class
    using Resource::Resource;

    DEFAULT_COPY_MOVE_OPERATIONS(Texture)

    // We keep both function overloads and only override one
    using Resource::Load;

    /// @brief Creates a texture using create info
    /// @param createInfo Creation info
    XNOR_ENGINE explicit Texture(const TextureCreateInfo& createInfo);

    /// @brief Creates a texture using a format and a size
    /// @param textureFormat Format
    /// @param size Size
    XNOR_ENGINE Texture(TextureInternalFormat textureFormat, Vector2i size);
    
    XNOR_ENGINE ~Texture() override;

    XNOR_ENGINE bool_t Load(const uint8_t* buffer, int64_t length) override;
    
    XNOR_ENGINE void CreateInRhi() override;

    XNOR_ENGINE void DestroyInRhi() override;
    
    XNOR_ENGINE void Unload() override;

    /// @brief Gets the raw data of the texture
    /// @tparam T Type
    /// @return Data
    template<typename T = char_t>
    [[nodiscard]]
    const T* GetData() const;

    /// @brief Gets the raw data of the texture
    /// @tparam T Type
    /// @return Data
    template<typename T = char_t>
    [[nodiscard]]
    T* GetData();

    /// @brief Gets the size of the texture
    /// @return Size
    [[nodiscard]]
    XNOR_ENGINE Vector2i GetSize() const;

    /// @brief Gets the number of channels of the file of the texture
    /// @return Number of channels
    [[nodiscard]]
    XNOR_ENGINE int32_t GetDataChannels() const;

    /// @brief Gets the loaded number of channels of the texture
    /// @return Number of channels
    [[nodiscard]]
    XNOR_ENGINE int32_t GetChannels() const;

    /// @brief Binds the texture
    /// @param index Index
    XNOR_ENGINE void BindTexture(uint32_t index) const;
    
    /// @brief Unbinds the texture
    /// @param index Index
    XNOR_ENGINE void UnbindTexture(uint32_t index) const;

    /// @brief Gets the texture id
    /// @return Texture id
    [[nodiscard]]
    XNOR_ENGINE uint32_t GetId() const;

    /// @brief Gets the filtering option
    /// @return Filtering
    XNOR_ENGINE TextureFiltering GetTextureFiltering() const;

    /// @brief Gets the wrapping option
    /// @return Wrapping option
    XNOR_ENGINE TextureWrapping GetTextureWrapping() const;
    
    /// @brief Gets the internal format
    /// @return Internal format
    XNOR_ENGINE TextureInternalFormat GetInternalFormat() const;

    /// @brief Gets the format
    /// @return Format
    XNOR_ENGINE TextureFormat GetTextureFormat() const;

private:
    uint8_t* m_Data = nullptr;
    Vector2i m_Size;
    int32_t m_DataChannels = 0;
    uint32_t m_Id = 0;

    TextureFiltering m_TextureFiltering = TextureFiltering::Linear;
    TextureWrapping m_TextureWrapping = TextureWrapping::Repeat;
    TextureInternalFormat m_TextureInternalFormat = TextureInternalFormat::Rgba8;
    TextureFormat m_TextureFormat = TextureFormat::Rgb;
};

END_XNOR_CORE

#include "resource/texture.inl"
