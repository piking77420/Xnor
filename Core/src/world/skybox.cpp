#include "world/skybox.hpp"

using namespace XnorCore;


void Skybox::LoadCubeMap(const std::array<std::string, 6>& cubeMapFiles)
{
    if(m_CubeMap != nullptr)
    {
        m_CubeMap->DestroyInRhi();
        m_CubeMap->Unload();
        delete m_CubeMap;
        m_CubeMap = nullptr;
    }

    m_CubeMap = new Cubemap(cubeMapFiles);
    m_CubeMap->CreateInRhi();
}

const Cubemap* Skybox::GetCubeMap() const
{
    return m_CubeMap;
}

const Cubemap* Skybox::GetCubeMap()
{
    return m_CubeMap;
}
