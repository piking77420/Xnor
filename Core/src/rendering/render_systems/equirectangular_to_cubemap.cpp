#include "rendering/render_systems/equirectangular_to_cubemap.hpp"

#include "rendering/frame_buffer.hpp"
#include "resource/resource_manager.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;

EquirectangularToCubeMap::EquirectangularToCubeMap()
{
    
    m_Cube = ResourceManager::Get("assets/models/cube.obj");
    //m_Shader = ResourceManager::Get()

}

EquirectangularToCubeMap::~EquirectangularToCubeMap()
{
    delete m_FrameBuffer;
    delete m_TextureDepth;
}


void EquirectangularToCubeMap::Compute(const Texture& equirectangularMap, const Cubemap& cubemap)
{
    
    if (m_FrameBuffer == nullptr ||m_FrameBuffer->GetSize() != cubemap.GetSize())
    {
        delete m_FrameBuffer;
        delete m_TextureDepth;
        m_FrameBuffer = new FrameBuffer(cubemap.GetSize());
    }

    Matrix projection;
    Matrix::Perspective(90.f * Calc::Deg2Rad, 1.0f,0.1f,10.f,&projection);

    /*
    std::array<Matrix,static_cast<constexpr size_t>(CubeMapFace::Size)> captureViews =
    {
        Matrix::LookAt(Vector3(),Vector3::UnitX(),-Vector3::UnitY()),
        Matrix::LookAt(Vector3(),-Vector3::UnitX(),-Vector3::UnitY()),
        Matrix::LookAt(Vector3(),Vector3::UnitY(),Vector3::UnitZ()),
        Matrix::LookAt(Vector3(),-Vector3::UnitX(),-Vector3::UnitY()),
        Matrix::LookAt(Vector3(),Vector3::UnitZ(),-Vector3::UnitY()),
        Matrix::LookAt(Vector3(),-Vector3::UnitZ(),-Vector3::UnitY()),

        
    } */

    
    for (size_t i = 0; i < static_cast<size_t>(CubeMapFace::Size); i++)
    {
        m_FrameBuffer->AttachTexture(cubemap,Attachment::Color00,static_cast<CubeMapFace>(i));

        
    }
    
}

void EquirectangularToCubeMap::InitCreateFrameBuffer(Vector2i size)
{
    m_FrameBuffer = new FrameBuffer(size);
    m_TextureDepth = new Texture(TextureInternalFormat::DepthComponent32,size);
    m_FrameBuffer->AttachTexture(*m_TextureDepth,Attachment::Depth);
}
