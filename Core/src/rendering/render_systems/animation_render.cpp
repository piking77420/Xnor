#include "rendering/render_systems/animation_render.hpp"

#include "input/time.hpp"
#include "rendering/animator.hpp"
#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"
#include "scene/scene.hpp"

using namespace XnorCore;

AnimationRender::AnimationRender()
    : m_SkinnedMeshGpuData(new SkinnedMeshGpuData())
{
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
    m_SkinnedShader->CreateInInterface();
}

void AnimationRender::BeginFrame(const Scene& scene, const Renderer&)
{
    scene.GetAllComponentOfType<SkinnedMeshRenderer>(&m_SkinnedRender);
}

void AnimationRender::RenderAnimation() const
{
    m_SkinnedShader->Use();

    for (const SkinnedMeshRenderer* skinnedMeshRender : m_SkinnedRender)
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

        if (skinnedMeshRender->mesh)
        {
            skinnedMeshRender->material.BindMaterial();

            for (uint32_t i = 0; i < skinnedMeshRender->mesh->models.GetSize(); i++)
            {
                const List<Matrix>& matrices = skinnedMeshRender->GetMatrices();
                for (size_t j = 0; j < matrices.GetSize(); j++)
                    m_SkinnedMeshGpuData->boneMatrices[j] = matrices[j];

                Rhi::UpdateAnimationUniform(*m_SkinnedMeshGpuData);
                Rhi::DrawModel(DrawMode::Triangles, skinnedMeshRender->mesh->models[i]->GetId());
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
