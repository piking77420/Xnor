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
    Rhi::CreateTexture2D(&m_Id, createInfo);
    m_LoadedInRhi = true;
}

Texture::Texture(const TextureInternalFormat textureInternalFormat, const Vector2i size) : m_Data(nullptr)
  , m_Size(size) , m_TextureInternalFormat(textureInternalFormat)
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
    
    Rhi::CreateTexture2D(&m_Id,createInfo);
    m_LoadedInRhi = true;
}

Texture::~Texture()
{
    if (m_LoadedInRhi)
    {
        Texture::DestroyInRhi();
    }
    
    if (m_Loaded)
    {
        Texture::Unload();
    }
}

void Texture::Load(const uint8_t* buffer, const int64_t length)
{
    stbi_set_flip_vertically_on_load(loadData.flipVertically);
    m_Data = stbi_load_from_memory(buffer, static_cast<int32_t>(length), &m_Size.x, &m_Size.y, &m_DataChannels, loadData.desiredChannels);
    m_TextureFormat = Rhi::GetFormat(m_DataChannels);
    m_Loaded = true;
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
    
    Rhi::CreateTexture2D(&m_Id, textureCreateInfo);
    
    m_LoadedInRhi = true;
}

void Texture::DestroyInRhi()
{
    Rhi::DestroyTexture(&m_Id);
    
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

void Texture::BindTexture([[maybe_unused]] const uint32_t index) const
{
    Rhi::BindTexture(index,m_Id);
}

void Texture::UnbindTexture(uint32_t index) const
{
    Rhi::BindTexture(index,0);
}

uint32_t Texture::GetId() const
{
    return m_Id;
}

const TextureFiltering Texture::GetTextureFiltering() const
{
    return m_TextureFiltering;
}

const TextureWrapping Texture::GetTextureWrapping() const
{
    return  m_TextureWrapping;
}

const TextureInternalFormat Texture::GetInternalFormat() const
{
    return m_TextureInternalFormat;
}

const TextureFormat Texture::GetTextureFormat() const
{
    return m_TextureFormat;
}

