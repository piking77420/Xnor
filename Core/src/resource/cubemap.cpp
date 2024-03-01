#include "resource/cubemap.hpp"

#include "resource/texture.hpp"
#include "rendering/rhi.hpp"


using namespace XnorCore;

void Cubemap::Load([[maybe_unused]] const uint8_t* buffer, [[maybe_unused]]int64_t length)
{
    // TO do how to manager 6 resource texture with one buffer
    Logger::LogError("Non Implemented function");
}

void Cubemap::Load([[maybe_unused]]const Pointer<File>& file)
{
    // Could work with meta data file
    Logger::LogError("Non Implemented function");
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
    for (uint32_t i = 0; i < m_Datas.size(); i++)
    {
        Texture::FreeData(m_Datas[i]); 
    }
    m_Loaded = false;
}

Cubemap::Cubemap(const std::array<std::string, 6>& cubeMapsTextures)
{
    for (size_t i = 0; i < cubeMapsTextures.size(); i++)
    {
       m_Datas[i] = Texture::LoadDataFromFile(cubeMapsTextures[i].c_str(), &m_CubeMapSize.x, &m_CubeMapSize.y, &m_DataChannels, 0);
    }
    m_Loaded = true;
}

void Cubemap::BindTexture(uint32_t unit) const
{
    Rhi::BindTexture(unit,m_Id);
}

void Cubemap::UnBindTexture(uint32_t unit) const
{
    Rhi::BindTexture(unit,0);
}


