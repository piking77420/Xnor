#include "world/skybox.hpp"


using namespace XnorCore;


void Skybox::Initialize()
{
    m_EquirectangularToCubeMap.InitResource();
}

void Skybox::LoadCubeMap(const std::array<std::string, 6>& cubeMapFiles)
{
    if (m_CubeMap != nullptr)
    {
        m_CubeMap->DestroyInRhi();
        m_CubeMap->Unload();
        delete m_CubeMap;
        m_CubeMap = nullptr;
    }

    m_CubeMap = new Cubemap(cubeMapFiles);
    m_CubeMap->CreateInRhi();
}

void Skybox::LoadFromHdrTexture(const Pointer<Texture>& hdfFile)
{
    if (m_CubeMap != nullptr)
    {
        delete m_CubeMap;
    }
    
    CreateCubeMapInfo createCubeMapInfo =
    {
        .datas = nullptr,
        .size = m_EnvironementCubeMapSize,
        .filtering = TextureFiltering::Linear,
        .wrapping = TextureWrapping::ClampToEdge,
        .format = TextureFormat::Rgb,
        .internalFormat = TextureInternalFormat::Rgb16F,
        .dataType = DataType::Float
    };
    m_CubeMap = new Cubemap(createCubeMapInfo);
    
    m_EquirectangularToCubeMap.Compute(*hdfFile.Get(), *m_CubeMap);
}

const Cubemap* Skybox::GetCubeMap() const
{
    return m_CubeMap;
}

Cubemap* Skybox::GetCubeMap()
{
    return m_CubeMap;
}
