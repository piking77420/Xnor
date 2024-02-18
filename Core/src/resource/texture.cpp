#include "resource/texture.hpp"

#include <stb/stb_image.h>

#include "rendering/rhi.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;



Texture::Texture(TextureCreateInfo& createInfo) : m_Data(reinterpret_cast<uint8_t*>(createInfo.data))
,m_Size({static_cast<int>(createInfo.textureSizeWidth),static_cast<int>(createInfo.textureSizeHeight)})
,m_TextureFiltering(createInfo.textureFiltering),m_TextureWrapping(createInfo.textureWrapping)
,m_TextureInternalFormat(createInfo.textureInternalFormat)
{
    RHI::CreateTexture(&m_Id, createInfo);
    m_Loaded = true;
}

Texture::Texture(Attachements attachements,vec2i size)
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
        case Attachements::COLOR:
            createInfo.textureInternalFormat = TextureInternalFormat::RGBA_16F;
            break;
        
        case Attachements::POSITION:
            createInfo.textureInternalFormat = TextureInternalFormat::RGBA_16F;
            break;
        
        case Attachements::NORMAL:
            createInfo.textureInternalFormat = TextureInternalFormat::RGBA_16F;
            break;
        
        case Attachements::TEXTURECOORD:
            createInfo.textureInternalFormat = TextureInternalFormat::RG_16;
            break;
    }
    
    RHI::CreateTexture(&m_Id,createInfo);
    m_Loaded = true;
}

Texture::~Texture()
{
    RHI::DestroyTexture(&m_Id);
}

void Texture::Load(File& file)
{
    stbi_set_flip_vertically_on_load(true);
    m_Data = stbi_load(file.GetFilepath().generic_string().c_str(), &m_Size.x, &m_Size.y, &m_Channels,0);

    TextureCreateInfo textureCreateInfo
    {
        m_Data,
        static_cast<uint32_t>(m_Size.x),
        static_cast<uint32_t>(m_Size.y),
        m_TextureFiltering,
        m_TextureWrapping,
        GetFormat(m_Channels),
        m_TextureInternalFormat
    };
    
    RHI::CreateTexture(&m_Id,textureCreateInfo);
    
    m_Loaded = true;
}

void Texture::Load([[maybe_unused]] const uint8_t* buffer, [[maybe_unused]] const int64_t length)
{
}

void Texture::Unload()
{
    
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

void Texture::BindTexture([[maybe_unused]] const uint32_t index)
{
    RHI::BindTexture(index,m_Id);
}

const uint32_t Texture::GetID() const
{
    return m_Id;
}

TextureFormat Texture::GetFormat(uint32_t textureFormat)
{
    switch (textureFormat)
    {
    case 1:
       return  TextureFormat::RED;
    case 3:
        return  TextureFormat::RGB;
    case 4:
         return  TextureFormat::RGBA;
    default:
        return  TextureFormat::RGB;
    }
    
}
