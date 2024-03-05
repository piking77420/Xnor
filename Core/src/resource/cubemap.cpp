#include "resource/cubemap.hpp"

#include <stb/stb_image.h>

#include "rendering/rhi.hpp"
#include "resource/texture.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

void Cubemap::CreateInRhi()
{
    const CreateCubeMapInfo createCubeMapInfo
    {
        .datas = &m_Images,
        .textureSizeWidth = static_cast<uint32_t>(m_CubeMapSize.x),
        .textureSizeHeight =  static_cast<uint32_t>(m_CubeMapSize.y),
        .textureFiltering = TextureFiltering::Linear,
        .textureWrapping = TextureWrapping::ClampToEdge,
        .textureFormat = Rhi::GetTextureFormatFromChannels(m_DataChannels),
        .textureInternalFormat = TextureInternalFormat::Rgba8,
        .dataType = DataType::UnsignedByte
    };

    Rhi::CreateCubeMap(&m_Id, createCubeMapInfo);
    m_LoadedInRhi = true;
}

void Cubemap::DestroyInRhi()
{
    Rhi::DestroyTexture(m_Id);
    m_LoadedInRhi = false;
}

void Cubemap::Unload()
{
    for (size_t i = 0; i < m_Images.size(); i++) // NOLINT(modernize-loop-convert)
    {
        stbi_image_free(m_Images[i]);
    }

    m_Loaded = false;
}

Cubemap::Cubemap(const std::array<std::string, 6>& cubeMapsTextures)
{
    stbi_set_flip_vertically_on_load(false);

    for (size_t i = 0; i < cubeMapsTextures.size(); i++)
    {
        m_Images[i] = stbi_load(cubeMapsTextures[i].c_str(), &m_CubeMapSize.x, &m_CubeMapSize.y, &m_DataChannels, 0);
    }
    
    m_Loaded = true;
}

void Cubemap::BindTexture(const uint32_t unit) const
{
    Rhi::BindTexture(unit, m_Id);
}

void Cubemap::UnBindTexture(const uint32_t unit) const
{
    Rhi::BindTexture(unit, 0);
}


