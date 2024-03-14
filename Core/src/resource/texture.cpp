#include "resource/texture.hpp"

#include <stb/stb_image.h>

#include "rendering/rhi.hpp"

using namespace XnorCore;

Texture::Texture(const TextureCreateInfo& createInfo)
    : m_Data(static_cast<uint8_t*>(createInfo.data))
    , m_Size(createInfo.size)
    , m_TextureFiltering(createInfo.filtering), m_TextureWrapping(createInfo.wrapping)
    , m_TextureInternalFormat(createInfo.internalFormat)
{
    m_Id = Rhi::CreateTexture2D(createInfo);
    m_LoadedInRhi = true;
}

Texture::Texture(const TextureInternalFormat textureFormat, const Vector2i size)
    : m_Size(size), m_TextureInternalFormat(textureFormat)
{
    const TextureCreateInfo createInfo
    {
        .data = nullptr,
        .size = size,
        .filtering = m_TextureFiltering,
        .wrapping = m_TextureWrapping,
        .format = m_TextureFormat,
        .internalFormat = m_TextureInternalFormat
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
    
    if (std::filesystem::path(m_Name).extension() == ".hdr")
    {
        m_Data = reinterpret_cast<decltype(m_Data)>(stbi_loadf_from_memory(buffer, static_cast<int32_t>(length), &m_Size.x, &m_Size.y, &m_DataChannels, loadData.desiredChannels));
    }
    else
    {
        m_Data = stbi_load_from_memory(buffer, static_cast<int32_t>(length), &m_Size.x, &m_Size.y, &m_DataChannels, loadData.desiredChannels);
    }
    
    m_TextureFormat = Rhi::GetTextureFormatFromChannels(m_DataChannels);
    m_Loaded = true;
    return true;
}

void Texture::CreateInRhi()
{
    TextureCreateInfo createInfo
   {
       .data = m_Data,
       .size = m_Size,
       .filtering = m_TextureFiltering,
       .wrapping = m_TextureWrapping,
       .format = m_TextureFormat,
       .internalFormat = m_TextureInternalFormat,
       .dataType = DataType::UnsignedByte
   };

    if (std::filesystem::path(m_Name).extension() == ".hdr")
    {
        createInfo.filtering = TextureFiltering::Linear;
        createInfo.wrapping = TextureWrapping::ClampToEdge;
        createInfo.internalFormat = TextureInternalFormat::Rgb16F;
        createInfo.format = TextureFormat::Rgb;
        createInfo.dataType = DataType::Float;
    }
    
    m_Id = Rhi::CreateTexture2D(createInfo);
    
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

