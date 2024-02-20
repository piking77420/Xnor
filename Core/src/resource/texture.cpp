#include "resource/texture.hpp"

#include <stb/stb_image.h>

#include "rendering/rhi.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

Texture::Texture(const TextureCreateInfo& createInfo)
    : m_Data(static_cast<uint8_t*>(createInfo.data))
    , m_Size(static_cast<int>(createInfo.textureSizeWidth), static_cast<int>(createInfo.textureSizeHeight))
    , m_TextureFiltering(createInfo.textureFiltering), m_TextureWrapping(createInfo.textureWrapping)
    , m_TextureInternalFormat(createInfo.textureInternalFormat)
{
    RHI::CreateTexture(&m_Id, createInfo);
    m_Loaded = true;
}

Texture::Texture(const AttachementsType attachements, const vec2i size)
{
    TextureCreateInfo createInfo
    {
        nullptr,
        static_cast<uint32_t>(size.x),
        static_cast<uint32_t>(size.y),
        TextureFiltering::LINEAR,
        TextureWrapping::NONE,
        TextureFormat::RGB,
        TextureInternalFormat::RGBA_16F
    };

    switch (attachements)
    {
        case AttachementsType::Color:
        case AttachementsType::Position:
        case AttachementsType::Normal:
            createInfo.textureInternalFormat = TextureInternalFormat::RGBA_16F;
            break;
        case AttachementsType::Texturecoord:
            createInfo.textureInternalFormat = TextureInternalFormat::RG_16;
            break;
        case AttachementsType::Depth:
            break;
        case AttachementsType::Stencil:
            break;
        case AttachementsType::DepthAndStencil:
            createInfo.textureInternalFormat = TextureInternalFormat::DEPTH_STENCIL;
            break;
    }
    
    RHI::CreateTexture(&m_Id,createInfo);
    m_Loaded = true;
}

Texture::~Texture()
{
    if (m_Loaded)
        Texture::Unload();
}

void Texture::Load(File& file)
{
    Load(file.GetData<uint8_t>(), file.GetSize());
}

void Texture::Load(const uint8_t* buffer, const int64_t length)
{
    stbi_set_flip_vertically_on_load(true);
    m_Data = stbi_load_from_memory(buffer, static_cast<int32_t>(length), &m_Size.x, &m_Size.y, &m_Channels, 0);

    const TextureCreateInfo textureCreateInfo
    {
        m_Data,
        static_cast<uint32_t>(m_Size.x),
        static_cast<uint32_t>(m_Size.y),
        m_TextureFiltering,
        m_TextureWrapping,
        GetFormat(m_Channels),
        m_TextureInternalFormat
    };
    
    RHI::CreateTexture(&m_Id, textureCreateInfo);
    
    m_Loaded = true;
}

void Texture::Unload()
{
    RHI::DestroyTexture(&m_Id);
    
    stbi_image_free(m_Data);
    m_Data = nullptr;
    m_Size = 0;
    
    m_Loaded = false;
}

const unsigned char* Texture::GetData() const
{
    return m_Data;
}

Vector2i Texture::GetSize() const
{
    return m_Size;
}

int Texture::GetChannels() const
{
    return m_Channels;
}

void Texture::BindTexture([[maybe_unused]] const uint32_t index) const
{
    RHI::BindTexture(index,m_Id);
}

uint32_t Texture::GetId() const
{
    return m_Id;
}

TextureFormat Texture::GetFormat(const uint32_t textureFormat)
{
    switch (textureFormat)
    {
        case 1:
           return TextureFormat::RED;
        case 3:
            return TextureFormat::RGB;
        case 4:
             return TextureFormat::RGBA;
        default:
            return TextureFormat::RGB;
    }
    
}
