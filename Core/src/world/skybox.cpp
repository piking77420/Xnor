#include "world/skybox.hpp"

using namespace XnorCore;

Skybox::~Skybox()
{
    delete cubeMap;
    delete irradianceMap;
    delete prefilterMap;
    delete precomputeBrdfTexture;
}

void Skybox::Initialize()
{
    m_SkyBoxParser.InitResource();
    delete cubeMap;
    delete cubeMap;
    delete irradianceMap;
    delete prefilterMap;
    delete precomputeBrdfTexture;
    
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
    cubeMap = new Cubemap(createCubeMapInfo);
    
    createCubeMapInfo.size = IradianceCubeSize;
    irradianceMap = new Cubemap(createCubeMapInfo);
    
    createCubeMapInfo.size = PrefilterMapSize;
    createCubeMapInfo.filtering = TextureFiltering::LinearMimMapLinear;
    prefilterMap = new Cubemap(createCubeMapInfo);

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
    precomputeBrdfTexture = new Texture(precomputeBrdf);
}

void Skybox::LoadCubeMap(const std::array<std::string, 6>& cubeMapFiles)
{
    if (cubeMap != nullptr)
    {
        cubeMap->DestroyInRhi();
        cubeMap->Unload();
        delete cubeMap;
        cubeMap = nullptr;
    }

    cubeMap = new Cubemap(cubeMapFiles);
    cubeMap->CreateInRhi();
}

void Skybox::LoadFromHdrTexture(const Pointer<Texture>& hdfFile)
{
    m_SkyBoxParser.EquirectangularToCubeMapFunc(*hdfFile.Get(), *cubeMap);
    m_SkyBoxParser.PreComputeBrdf(EnvironementCubeMapSize,*precomputeBrdfTexture);
    m_SkyBoxParser.ComputeIrradiance(*cubeMap,*irradianceMap, IradianceCubeSize);
    m_SkyBoxParser.ComputePreFiltering(*cubeMap,*prefilterMap,MaxMinMapLevel);
}
