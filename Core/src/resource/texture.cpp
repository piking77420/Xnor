#include "resource/texture.hpp"

#include <stb/stb_image.h>

#include "rendering/rhi.hpp"

using namespace XnorCore;

Texture::Texture(TextureCreateInfo createInfo)
{
    RHI::CreateTexture(&m_Id,createInfo);
    m_Loaded = true;
}

Texture::~Texture()
{
    RHI::DestroyTexture(&m_Id);
}

void Texture::Load(File& file)
{
    m_Data = stbi_load(file.GetFilepath().generic_string().c_str(), &m_Size.x, &m_Size.y, &m_Channels,0);

    TextureCreateInfo textureCreateInfo
    {
        m_Data,
        static_cast<uint32_t>(m_Size.x),
        static_cast<uint32_t>(m_Size.y),
        TextureFiltering::LINEAR,
        TextureWrapping::REPEAT,
        GetFormat(m_Channels)
    };
    
    RHI::CreateTexture(&m_Id,textureCreateInfo);
    
    m_Loaded = true;
}

void Texture::Load(const uint8_t* buffer, int64_t length)
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
    RHI::BindTexture(TextureType::TEXTURE_2D,m_Id);
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
