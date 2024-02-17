#include "resource/texture.hpp"

#include <stb/stb_image.h>
#include <glad/glad.h>

#include "rendering/rhi.hpp"

using namespace XnorCore;

Texture::~Texture()
{
    RHI::DestroyTexture(&m_Id);
}

void Texture::Load(const uint8_t* buffer, const int64_t length)
{
    m_Data = stbi_load_from_memory(buffer, static_cast<int32_t>(length), &m_Size.x, &m_Size.y, &m_Channels, desiredChannels);
    
    RHI::CreateTexture(&m_Id,m_Data,m_Size,TextureWrapping::REPEAT,TextureFiltering::LINEAR);
    
    m_Loaded = true;
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

void Texture::BindTexture(const uint32_t index)
{
    RHI::BindTexture(TextureType::TEXTURE_2D,m_Id);
}
