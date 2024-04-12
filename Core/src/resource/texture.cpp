#include "resource/texture.hpp"

#include <stb/stb_image.h>

#include "rendering/rhi.hpp"

using namespace XnorCore;

Texture::Texture(const TextureCreateInfo& createInfo)
    : m_Size(createInfo.size)
    , m_TextureFiltering(createInfo.filtering)
    , m_TextureWrapping(createInfo.wrapping)
    , m_TextureInternalFormat(createInfo.internalFormat)
{
    if (!createInfo.datas.empty())
        m_Data = static_cast<uint8_t*>(createInfo.datas.at(0));
    
    m_Id = Rhi::CreateTexture(createInfo);
    m_LoadedInRhi = true;
}

Texture::Texture(const ENUM_VALUE(TextureInternalFormat) textureInternalFormat, const Vector2i size, const ENUM_VALUE(TextureFormat) textureFormat)
    : m_Size(size)
    , m_TextureInternalFormat(textureInternalFormat)
    , m_TextureFormat(textureFormat)
{
    const TextureCreateInfo createInfo
    {
        .textureType = TextureType::Texture2D,
        .size = size,
        .filtering = m_TextureFiltering,
        .wrapping = m_TextureWrapping,
        .format = textureFormat,
        .internalFormat = m_TextureInternalFormat
    };
    
    m_Id = Rhi::CreateTexture(createInfo);
    m_LoadedInRhi = true;
}

Texture::~Texture()
{
    if (m_LoadedInRhi)
        Texture::DestroyInRhi();
    
    if (m_Loaded)
        Texture::Unload();
}

bool_t Texture::Load(const uint8_t* buffer, const int64_t length)
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
        .size = m_Size,
        .filtering = m_TextureFiltering,
        .wrapping = m_TextureWrapping,
        .format = m_TextureFormat,
        .internalFormat = m_TextureInternalFormat,
        .dataType = DataType::UnsignedByte
    };
    createInfo.datas.push_back(m_Data);

    if (std::filesystem::path(m_Name).extension() == ".hdr")
    {
        createInfo.filtering = TextureFiltering::Linear;
        createInfo.wrapping = TextureWrapping::ClampToEdge;
        createInfo.internalFormat = TextureInternalFormat::Rgb16F;
        createInfo.format = TextureFormat::Rgb;
        createInfo.dataType = DataType::Float;
    }
    
    m_Id = Rhi::CreateTexture(createInfo);
    
    m_LoadedInRhi = true;
}

void Texture::DestroyInRhi()
{
    Rhi::DestroyTexture(m_Id);
    
    m_LoadedInRhi = false;
}

void Texture::Unload()
{
    if (!m_IsEmbedded)
        stbi_image_free(m_Data);

    m_Data = nullptr;
    m_Size = Vector2i::Zero();
    
    m_Loaded = false;
}

void Texture::SetIsEmbedded()
{
    m_IsEmbedded = true;
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

TextureFiltering::TextureFiltering Texture::GetTextureFiltering() const
{
    return m_TextureFiltering;
}

TextureWrapping::TextureWrapping Texture::GetTextureWrapping() const
{
    return m_TextureWrapping;
}

TextureInternalFormat::TextureInternalFormat Texture::GetInternalFormat() const
{
    return m_TextureInternalFormat;
}

TextureFormat::TextureFormat Texture::GetTextureFormat() const
{
    return m_TextureFormat;
}

