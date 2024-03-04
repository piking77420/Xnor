#include "resource/cubemap.hpp"

#include <stb/stb_image.h>

#include "rendering/rhi.hpp"
#include "resource/texture.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

bool_t Cubemap::Load(const uint8_t*, int64_t)
{
    // TO do how to manager 6 resource texture with one buffer
    Logger::LogError("Non Implemented function");
    return false;
}

bool_t Cubemap::Load(const Pointer<File>&)
{
    // Could work with meta data file
    Logger::LogError("Non Implemented function");
    return false;
}

void Cubemap::CreateInRhi()
{
    const CreateCubeMapInfo createCubeMapInfo
    {
        .datas = &m_Datas,
        .textureSizeWidth = static_cast<uint32_t>(m_CubeMapSize.x),
        .textureSizeHeight =  static_cast<uint32_t>(m_CubeMapSize.y),
        .textureFiltering = TextureFiltering::Linear,
        .textureWrapping = TextureWrapping::ClampToEdge,
        .textureFormat = Rhi::GetFormat(m_DataChannels),
        .textureInternalFormat = TextureInternalFormat::Rgba8,
        .dataType = DataType::UnsignedByte
    };

    Rhi::CreateCubeMap(&m_Id,createCubeMapInfo);
    m_LoadedInRhi = true;
}

void Cubemap::DestroyInRhi()
{
    Rhi::DestroyTexture(&m_Id);
    m_LoadedInRhi = false;
}

void Cubemap::Unload()
{
    for (const auto& data : m_Datas)
    {
        stbi_image_free(data);
    }

    m_Loaded = false;
}

Cubemap::Cubemap(const std::array<std::string, 6>& cubeMapsTextures)
{
    stbi_set_flip_vertically_on_load(false);
    for (size_t i = 0; i < cubeMapsTextures.size(); i++)
    {
        m_Datas[i] = stbi_load(cubeMapsTextures[i].c_str(), &m_CubeMapSize.x, &m_CubeMapSize.y, &m_DataChannels, 0);
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


