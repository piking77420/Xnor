#include "rendering/render_systems/equirectangular_to_cubemap.hpp"

#include "rendering/frame_buffer.hpp"
#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"
#include "utils/logger.hpp"

using namespace XnorCore;


EquirectangularToCubeMap::~EquirectangularToCubeMap()
{
    delete m_FrameBuffer;
    delete m_TextureDepth;
}

void EquirectangularToCubeMap::EquirectangularToCubeMapFunc(const Texture& equirectangularMap, const Cubemap& cubemap)
{
    Compute(equirectangularMap,cubemap,m_EquirectangularToCubeMapShader);
}



void EquirectangularToCubeMap::Compute(const Texture& equirectangularMap, const Cubemap& cubemap, const Pointer<Shader>& shader)
{
    
    if (m_FrameBuffer == nullptr ||m_FrameBuffer->GetSize() != cubemap.GetSize())
    {
        delete m_FrameBuffer;
        delete m_TextureDepth;
        InitCreateFrameBuffer(cubemap.GetSize());
    }

    Matrix projection;
    Matrix::Perspective(90.f * Calc::Deg2Rad, 1.0f,0.1f,10.f,&projection);
    
    std::array<Matrix,static_cast<size_t>(CubeMapFace::Size)> captureViews =
    {
        Matrix::LookAt(Vector3(),-Vector3::UnitX(),-Vector3::UnitY()), // CubeMapPositiveX
        Matrix::LookAt(Vector3(),Vector3::UnitX(),-Vector3::UnitY()), // CubeMapNegativeX
        Matrix::LookAt(Vector3(),-Vector3::UnitY(),-Vector3::UnitZ()), // CubeMapPositiveY
        Matrix::LookAt(Vector3(),Vector3::UnitY(),Vector3::UnitZ()), // CubeMapNegativeY
        Matrix::LookAt(Vector3(),Vector3::UnitZ(),-Vector3::UnitY()), // CubeMapPositiveZ
        Matrix::LookAt(Vector3(),-Vector3::UnitZ(),-Vector3::UnitY()), // CubeMapNegativeZ
    };

    shader->Use();
    shader->SetMat4("projection",projection);
    equirectangularMap.BindTexture(0);
    
    for (size_t i = 0; i < 6; i++)
    {
        RenderPassBeginInfo renderPassBeginInfo =
        {
            .frameBuffer = m_FrameBuffer,
            .renderAreaOffset = { 0,0 },
            .renderAreaExtent = m_FrameBuffer->GetSize() ,
            .clearBufferFlags =  static_cast<decltype(renderPassBeginInfo.clearBufferFlags)>(BufferFlagColorBit | BufferFlagDepthBit),
            .clearColor = Vector4()
        };
        shader->SetMat4("view",captureViews[i]);

        m_FrameBuffer->AttachTexture(cubemap,Attachment::Color00,static_cast<CubeMapFace>(i));
        m_RenderPass.BeginRenderPass(renderPassBeginInfo);

        Rhi::DrawModel(m_Cube->GetId());

        m_RenderPass.EndRenderPass();
    }

    shader->Unuse();
}

void EquirectangularToCubeMap::InitResource()
{
    m_Cube = ResourceManager::Get<Model>("assets/models/cube.obj");
    
    m_EquirectangularToCubeMapShader = ResourceManager::Get<Shader>("equirectangular_to_cubemap");
    m_EquirectangularToCubeMapShader->SetDepthFunction(DepthFunction::LessEqual);
    m_EquirectangularToCubeMapShader->CreateInRhi();
    m_EquirectangularToCubeMapShader->SetInt("equirectangularMap",0);

    
}

void EquirectangularToCubeMap::InitCreateFrameBuffer(Vector2i size)
{
    m_FrameBuffer = new FrameBuffer(size);
    m_TextureDepth = new Texture(TextureInternalFormat::DepthComponent32,size);
    m_TextureDepth->CreateInRhi();
    m_FrameBuffer->AttachTexture(*m_TextureDepth,Attachment::Depth);
    //m_color = new Texture(TextureInternalFormat::Rgb16,size);
    //m_FrameBuffer->AttachTexture(*m_color,Attachment::Color00);

}

