#pragma once

#include <Maths/vector2i.hpp>

#include "core.hpp"
#include "resource.hpp"
#include "rendering/rhi_typedef.hpp"

BEGIN_XNOR_CORE

struct TextureLoadData
{
    int32_t desiredChannels = 0;
    bool_t flipVertically = true;
};

class Texture : public Resource
{
public:
    TextureLoadData loadData;
    
    // Same constructor from base class
    using Resource::Resource;

    DEFAULT_COPY_MOVE_OPERATIONS(Texture)

    // We keep both function overloads and only override one
    using Resource::Load;

    XNOR_ENGINE explicit Texture(const TextureCreateInfo& createInfo);

    XNOR_ENGINE Texture(AttachementsType attachements, vec2i size);
    
    XNOR_ENGINE ~Texture() override;
    
    XNOR_ENGINE void Load(File& file) override;
    
    XNOR_ENGINE void Load(const uint8_t* buffer, int64_t length) override;
    
    XNOR_ENGINE void Unload() override;

    template<typename T = char_t>
    [[nodiscard]]
    const T* GetData() const;

    template<typename T = char_t>
    [[nodiscard]]
    T* GetData();

    [[nodiscard]]
    XNOR_ENGINE Vector2i GetSize() const;

    [[nodiscard]]
    XNOR_ENGINE int32_t GetDataChannels() const;

    [[nodiscard]]
    XNOR_ENGINE int32_t GetChannels() const;

    XNOR_ENGINE virtual void BindTexture(uint32_t index) const;
    
    XNOR_ENGINE uint32_t GetId() const;

private:
    uint8_t* m_Data = nullptr;
    Vector2i m_Size;
    int32_t m_DataChannels = 0;
    uint32_t m_Id = 0;

    TextureFiltering m_TextureFiltering = TextureFiltering::Linear;
    TextureWrapping m_TextureWrapping = TextureWrapping::Repeat;
    TextureInternalFormat m_TextureInternalFormat = TextureInternalFormat::Rgba8;

    static TextureFormat GetFormat(uint32_t textureFormat);
};

template<typename T>
const T* Texture::GetData() const
{
    return reinterpret_cast<const T*>(m_Data);
}

template<typename T>
T* Texture::GetData()
{
    return reinterpret_cast<T*>(m_Data);
}

END_XNOR_CORE
