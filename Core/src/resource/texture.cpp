#include "resource/texture.hpp"

#include <stb/stb_image.h>

using namespace XnorCore;

Texture::Texture(const std::string& filepath)
    : Resource(filepath)
{
}

void Texture::Load()
{
    Load(4);
}

void Texture::Load(const int desiredChannels)
{
    m_Data = stbi_load(m_Filepath.string().c_str(), &m_Size.x, &m_Size.y, &m_Channels, desiredChannels);
}

void Texture::Unload()
{
    stbi_image_free(m_Data);
    
    Resource::Unload();
}

const unsigned char* Texture::GetData() const
{
    return m_Data;
}

vec2i Texture::GetSize() const
{
    return m_Size;
}

int Texture::GetChannels() const
{
    return m_Channels;
}
