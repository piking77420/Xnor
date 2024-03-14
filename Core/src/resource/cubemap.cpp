#include "resource/cubemap.hpp"

#include <stb/stb_image.h>

#include "rendering/rhi.hpp"
#include "resource/texture.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

void Cubemap::CreateInRhi()
{
    const CreateCubeMapInfo createInfo
    {
        .datas = &m_Images,
        .size = m_CubemapSize,
        .filtering = TextureFiltering::Linear,
        .wrapping = TextureWrapping::ClampToEdge,
        .format = Rhi::GetTextureFormatFromChannels(m_DataChannels),
        .internalFormat = TextureInternalFormat::Rgba8,
        .dataType = DataType::UnsignedByte
    };

    m_Id = Rhi::CreateCubeMap(createInfo);
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
        stbi_image_free(m_Images[i]);

    m_Loaded = false;
}

Cubemap::Cubemap(const std::array<std::string, 6>& cubeMapsTextures)
{
    stbi_set_flip_vertically_on_load(false);

    for (size_t i = 0; i < cubeMapsTextures.size(); i++)
        m_Images[i] = stbi_load(cubeMapsTextures[i].c_str(), &m_CubemapSize.x, &m_CubemapSize.y, &m_DataChannels, 0);
    
    m_Loaded = true;
}

Cubemap::Cubemap(const CreateCubeMapInfo& createCubeMapInfo) : m_CubemapSize(createCubeMapInfo.size),
m_TextureFiltering(createCubeMapInfo.filtering),m_TextureWrapping(createCubeMapInfo.wrapping), m_TextureInternalFormat(createCubeMapInfo.internalFormat)
{
    m_Id = Rhi::CreateCubeMap(createCubeMapInfo);
    m_LoadedInRhi = true;
}

void Cubemap::BindTexture(const uint32_t unit) const
{
    Rhi::BindTexture(unit, m_Id);
}

void Cubemap::UnBindTexture(const uint32_t unit) const
{
    Rhi::BindTexture(unit, 0);
}
