#include "resource/texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glad/glad.h>

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

    // TO DO MOVE TO RHI

    glCreateTextures(GL_TEXTURE_2D, 1, &m_Id);

    glTextureParameteri(m_Id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(m_Id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_Id, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureStorage2D(m_Id, 1, GL_RGBA8, m_Size.x, m_Size.y);
    glTextureSubImage2D(m_Id, 0, 0, 0, m_Size.x, m_Size.y, GL_RGBA, GL_UNSIGNED_BYTE, m_Data);
    glGenerateTextureMipmap(m_Id);

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

void Texture::BindTexture(uint32_t index)
{
    glBindTextureUnit(0, m_Id);
}
