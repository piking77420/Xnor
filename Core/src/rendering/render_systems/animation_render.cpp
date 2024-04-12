#include "rendering/render_systems/animation_render.hpp"

#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"
#include "scene/scene.hpp"

using namespace XnorCore;

AnimationRender::AnimationRender()
{
    m_SkinnedMeshGpuData = new SkinnedMeshGpuData;
}

AnimationRender::~AnimationRender()
{
    delete m_SkinnedMeshGpuData;
}

void AnimationRender::InitResources()
{
    m_SkinnedShader = ResourceManager::Get<Shader>("skinned_gbuffer");
    constexpr ShaderProgramCullInfo cullInfo =
    {
        .enableCullFace = true,
        .cullFace = CullFace::Front,
        .frontFace = FrontFace::CCW
    };

    m_SkinnedShader->SetFaceCullingInfo(cullInfo);
    m_SkinnedShader->CreateInRhi();
}

void AnimationRender::BeginFrame(const Scene& scene, const Renderer&)
{
    scene.GetAllComponentOfType<SkinnedMeshRender>(&m_SkinnedRender);
}

void AnimationRender::RenderAnimation() const
{
    m_SkinnedShader->Use();
    for (const SkinnedMeshRender* skinnedMeshRender : m_SkinnedRender)
    {
        ModelUniformData modelData;
        modelData.model = skinnedMeshRender->GetTransform().worldMatrix;

        try
        {
            modelData.normalInvertMatrix = skinnedMeshRender->GetTransform().worldMatrix.Inverted().Transposed();
        }
        catch (const std::invalid_argument&)
        {
            modelData.normalInvertMatrix = Matrix::Identity();
        }
		
        Rhi::UpdateModelUniform(modelData);

        Rhi::BindMaterial(skinnedMeshRender->mesh->material);
        skinnedMeshRender->mesh->material.BindMaterial();

        if (skinnedMeshRender->mesh)
        {
            for (uint32_t i = 0; i < skinnedMeshRender->mesh->models.GetSize(); i++)
            {
                // TODO Compute matrices and set it to the struct m_SkinnedMeshGpuData;
                // m_SkinnedMeshGPUData
                // Set data in Shader

                for (size_t j = 0; j < MaxBones; j++)
                    m_SkinnedMeshGpuData->boneMatrices[j] = Matrix::Identity();

                Rhi::UpdateAnimationUniform(*m_SkinnedMeshGpuData);
                Rhi::DrawModel(skinnedMeshRender->mesh->models[i]->GetId());
            }
        }
    }
    m_SkinnedShader->Unuse();
}

void AnimationRender::EndFrame()
{
    // TO DO
    // In case of doing something at the end of frame 
}
