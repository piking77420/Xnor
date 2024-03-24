#include "rendering/render_systems/post_process_pass.hpp"

#include "rendering/bloom_rendertarget.hpp"
#include "rendering/postprocess_rendertarget.hpp"
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

void PostProcessPass::Compute(const Texture& textureToCompute, const Texture& postProcessedTexture, const PostprocessRendertarget& postprocessRendertarget) const

{
    
    //HandleResize(textureToCompute.GetSize());
    //m_BloomPass.ComputeBloom(textureToCompute);
    /*
    
    m_FrameBuffer->AttachTexture(postProcessedTexture, Attachment::Color00, 0);
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
    m_RenderPass.EndRenderPass();*/
}
