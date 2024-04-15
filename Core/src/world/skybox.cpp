#include "world/skybox.hpp"

#include "rendering/rhi.hpp"

using namespace XnorCore;

Skybox::~Skybox()
{
    delete m_CubeMap;
    m_CubeMap = nullptr;
    delete m_IrradianceMap;
    m_IrradianceMap = nullptr;
    delete m_PrefilterMap;
    m_IrradianceMap = nullptr;
    delete m_PrecomputeBrdfTexture;
    m_PrecomputeBrdfTexture = nullptr;
}

void Skybox::Initialize()
{
    delete m_CubeMap;
    delete m_IrradianceMap;
    delete m_PrefilterMap;
    delete m_PrecomputeBrdfTexture;
    
    TextureCreateInfo createCubeMapInfo =
    {
        .textureType = TextureType::TextureCubeMap,
        .mipMaplevel = 0,
        .size = EnvironementCubeMapSize,
        .filtering = TextureFiltering::Linear,
        .wrapping = TextureWrapping::ClampToEdge,
        .format = TextureFormat::Rgb,
        .internalFormat = TextureInternalFormat::Rgb16F,
        .dataType = DataType::Float
    };
    m_CubeMap = new Cubemap(createCubeMapInfo);
    
    createCubeMapInfo.size = IradianceCubeSize;
    m_IrradianceMap = new Cubemap(createCubeMapInfo);
    
    createCubeMapInfo.size = PrefilterMapSize;
    createCubeMapInfo.filtering = TextureFiltering::LinearMimMapLinear;
    m_PrefilterMap = new Cubemap(createCubeMapInfo);

    const TextureCreateInfo precomputeBrdf =
    {
        .textureType = TextureType::Texture2D,
        .size = EnvironementCubeMapSize,
        .filtering = TextureFiltering::Linear,
        .wrapping = TextureWrapping::ClampToEdge,
        .format = TextureFormat::RedGreen,
        .internalFormat = TextureInternalFormat::Rg16F,
        .dataType = DataType::Float
    };
    m_PrecomputeBrdfTexture = new Texture(precomputeBrdf);
}

void Skybox::LoadCubeMap(const std::array<std::string, 6>& cubeMapFiles)
{
    if (m_CubeMap != nullptr)
    {
        m_CubeMap->DestroyInInterface();
        m_CubeMap->Unload();
        delete m_CubeMap;
        m_CubeMap = nullptr;
    }

    m_CubeMap = new Cubemap(cubeMapFiles);
    m_CubeMap->CreateInInterface();
}

void Skybox::LoadFromHdrTexture(const Pointer<Texture>& hdfFile) const 
{
    Rhi::skyBoxParser.EquirectangularToCubeMapFunc(*hdfFile.Get(), *m_CubeMap);
    Rhi::skyBoxParser.PreComputeBrdf(EnvironementCubeMapSize, *m_PrecomputeBrdfTexture);
    Rhi::skyBoxParser.ComputeIrradiance(*m_CubeMap, *m_IrradianceMap, IradianceCubeSize);
    Rhi::skyBoxParser.ComputePreFiltering(*m_CubeMap, *m_PrefilterMap, MaxMinMapLevel);
}

void Skybox::BindDesriptorSet() const 
{
    m_IrradianceMap->BindTexture(DefferedDescriptor::SkyboxIrradiance);
    m_PrefilterMap->BindTexture(DefferedDescriptor::SkyboxPrefilterMap);
    m_PrecomputeBrdfTexture->BindTexture(DefferedDescriptor::SkyboxPrecomputeBrdf);
}

void Skybox::UnbindDesriptorSet() const
{
    m_IrradianceMap->UnBindTexture(DefferedDescriptor::SkyboxIrradiance);
    m_PrefilterMap->UnBindTexture(DefferedDescriptor::SkyboxPrefilterMap);
    m_PrecomputeBrdfTexture->UnbindTexture(DefferedDescriptor::SkyboxPrecomputeBrdf);
}

const Cubemap* Skybox::GetSkyboxAlbedoColor() const
{
    return m_CubeMap;
}

