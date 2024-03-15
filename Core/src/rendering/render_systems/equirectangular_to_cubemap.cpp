#include "rendering/render_systems/equirectangular_to_cubemap.hpp"

#include "rendering/frame_buffer.hpp"
#include "resource/resource_manager.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

void EquirectangularToCubeMap::Initalize()
{
    m_Cube = ResourceManager::Get("assets/models/cube.obj");
    //m_Shader = ResourceManager::Get()
}

void EquirectangularToCubeMap::Shutdown()
{
    
}

void EquirectangularToCubeMap::Compute(const Texture& equirectangularMap, const Cubemap& cubemap)
{
    
    if (m_FrameBuffer == nullptr ||m_FrameBuffer->GetSize() != cubemap.GetSize())
    {
        delete m_FrameBuffer;
        m_FrameBuffer = new FrameBuffer(cubemap.GetSize());
    }
    
    for (uint32_t i = 0; i < static_cast<uint32_t>(CubeMapFace::Size); i++)
    {
        m_FrameBuffer->AttachTexture(cubemap,Attachment::Color00,static_cast<CubeMapFace>(i));

        
    }
    
}
