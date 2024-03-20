#include "rendering/render_systems/post_process_pass.hpp"

#include "input/time.hpp"
#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"

using namespace XnorCore;

void PostProcessPass::HandleResize(Vector2i size)
{
    if(m_FrameBuffer != nullptr)
    {
        if (m_FrameBuffer->GetSize() != size)
        {
            delete m_FrameBuffer;
            m_FrameBuffer = nullptr;
            m_FrameBuffer = new FrameBuffer(size);
            return;
        }
        return;
    }
    m_FrameBuffer = new FrameBuffer(size);
}

PostProcessPass::~PostProcessPass()
{
    delete m_FrameBuffer;
    m_FrameBuffer = nullptr;
}

void PostProcessPass::Init()
{
    m_ToneMapping.InitializeResources();
    m_BloomPass.Init(5);
}

void PostProcessPass::Compute(const Texture& textureToCompute, const Texture& PostProcessedTexture)
{
    if (!enable)
        return;
    
    HandleResize(textureToCompute.GetSize());
    m_BloomPass.ComputeBloom(textureToCompute);
    
    
    m_FrameBuffer->AttachTexture(PostProcessedTexture, Attachment::Color00, 0);
    const RenderPassBeginInfo beginInfo =
    {
        .frameBuffer = m_FrameBuffer,
        .renderAreaOffset = { 0,0 },
        .renderAreaExtent = m_FrameBuffer->GetSize(),
        .clearBufferFlags = BufferFlag::None,
        .clearColor = Vector4::Zero()
    };
    m_RenderPass.BeginRenderPass(beginInfo);
    m_ToneMapping.ComputeToneMaping(textureToCompute,*m_BloomPass.GetBloomTexture());
    m_RenderPass.EndRenderPass();

    static bool_t init = [&]() -> bool_t
    {
        m_TestCompute = XnorCore::ResourceManager::Get<Shader>("test_compute");
        m_TestCompute->CreateInRhi();
        m_TestCompute->Use();
        m_TestCompute->SetInt("imgOutput",0);
        m_TestCompute->Unuse();
        return true;
    }();

    
    m_TestCompute->Use();
    
    textureToCompute.BindTexture(0);
    Rhi::BindImageTexture(0,textureToCompute.GetId(), 0, false, 0, ImageAccess::ReadWrite, TextureInternalFormat::Rgba32F);
    m_TestCompute->SetFloat("t",Time::GetTotalTime<float>());
    Rhi::DispactCompute(textureToCompute.GetSize().x / 10,textureToCompute.GetSize().y /10,1);
    Rhi::GpuMemoryBarrier(MemoryBarrier::AllBarrierBits);
    

    
}
