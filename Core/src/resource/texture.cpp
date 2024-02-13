#include "resource/texture.hpp"

#include <stb/stb_image.h>

#include <glad/glad.h>

using namespace XnorCore;

void Texture::Load(const uint8_t* buffer, const int64_t length)
{
    m_Data = stbi_load_from_memory(buffer, static_cast<int32_t>(length), &m_Size.x, &m_Size.y, &m_Channels, desiredChannels);
    
    glCreateTextures(GL_TEXTURE_2D, 1, &m_Id);

    glTextureParameteri(m_Id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(m_Id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_Id, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureStorage2D(m_Id, 1, GL_RGBA8, m_Size.x, m_Size.y);
    glTextureSubImage2D(m_Id, 0, 0, 0, m_Size.x, m_Size.y, GL_RGBA, GL_UNSIGNED_BYTE, m_Data);
    glGenerateTextureMipmap(m_Id);

    m_Loaded = true;
}

void Texture::Load(const File& file)
{
    Load(file.GetData<uint8_t>(), file.GetSize());
}

void Texture::Unload()
{
    glDeleteTextures(1, &m_Id);
    
    // No need to free the data from stbi_load as it is allocated in our File wrapper
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

void Texture::BindTexture(uint32_t index)
{
    glBindTextureUnit(0, m_Id);
}
