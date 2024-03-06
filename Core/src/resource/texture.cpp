#include "resource/texture.hpp"

#include <stb/stb_image.h>

#include "rendering/rhi.hpp"

using namespace XnorCore;

Texture::Texture(const TextureCreateInfo& createInfo)
    : m_Data(static_cast<uint8_t*>(createInfo.data))
    , m_Size(static_cast<int>(createInfo.textureSizeWidth), static_cast<int>(createInfo.textureSizeHeight))
    , m_TextureFiltering(createInfo.textureFiltering), m_TextureWrapping(createInfo.textureWrapping)
    , m_TextureInternalFormat(createInfo.textureInternalFormat)
{
    m_Id = Rhi::CreateTexture2D(createInfo);
    m_LoadedInRhi = true;
}

Texture::Texture(const TextureInternalFormat textureFormat, const Vector2i size)
    : m_Size(size), m_TextureInternalFormat(textureFormat)
{
    const TextureCreateInfo createInfo
    {
        nullptr,
        static_cast<uint32_t>(size.x),
        static_cast<uint32_t>(size.y),
        m_TextureFiltering,
        m_TextureWrapping,
        m_TextureFormat,
        m_TextureInternalFormat
    };
    
    m_Id = Rhi::CreateTexture2D(createInfo);
    m_LoadedInRhi = true;
}

Texture::~Texture()
{
    if (m_LoadedInRhi)
        Texture::DestroyInRhi();
    
    if (m_Loaded)
        Texture::Unload();
}

bool Texture::Load(const uint8_t* buffer, const int64_t length)
{
    stbi_set_flip_vertically_on_load(loadData.flipVertically);
    m_Data = stbi_load_from_memory(buffer, static_cast<int32_t>(length), &m_Size.x, &m_Size.y, &m_DataChannels, loadData.desiredChannels);
    m_TextureFormat = Rhi::GetTextureFormatFromChannels(m_DataChannels);
    m_Loaded = true;
    return true;
}

void Texture::CreateInRhi()
{
    const TextureCreateInfo textureCreateInfo
    {
        m_Data,
        static_cast<uint32_t>(m_Size.x),
        static_cast<uint32_t>(m_Size.y),
        m_TextureFiltering,
        m_TextureWrapping,
        m_TextureFormat,
        m_TextureInternalFormat,
        DataType::UnsignedByte
    };
    
    m_Id = Rhi::CreateTexture2D(textureCreateInfo);
    
    m_LoadedInRhi = true;
}

void Texture::DestroyInRhi()
{
    Rhi::DestroyTexture(m_Id);
    
    m_LoadedInRhi = false;
}

void Texture::Unload()
{
    stbi_image_free(m_Data);
    m_Data = nullptr;
    m_Size = Vector2i::Zero();
    
    m_Loaded = false;
}

Vector2i Texture::GetSize() const
{
    return m_Size;
}

int32_t Texture::GetDataChannels() const
{
    return m_DataChannels;
}

int32_t Texture::GetChannels() const
{
    return loadData.desiredChannels != 0 ? loadData.desiredChannels : m_DataChannels;
}

void Texture::BindTexture(const uint32_t index) const
{
    Rhi::BindTexture(index, m_Id);
}

void Texture::UnbindTexture(const uint32_t index) const
{
    Rhi::BindTexture(index, 0);
}

uint32_t Texture::GetId() const
{
    return m_Id;
}

TextureFiltering Texture::GetTextureFiltering() const
{
    return m_TextureFiltering;
}

TextureWrapping Texture::GetTextureWrapping() const
{
    return m_TextureWrapping;
}

TextureInternalFormat Texture::GetInternalFormat() const
{
    return m_TextureInternalFormat;
}

TextureFormat Texture::GetTextureFormat() const
{
    return m_TextureFormat;
}

