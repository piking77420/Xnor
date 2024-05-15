#include "rendering/render_systems/meshes_drawer.hpp"

#include "rendering/frustum.hpp"
#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"
#include "world/world.hpp"


using namespace XnorCore;

MeshesDrawer::MeshesDrawer()
    : m_SkinnedMeshGpuData(new SkinnedMeshGpuData())
{
}

MeshesDrawer::~MeshesDrawer()
{
    delete m_SkinnedMeshGpuData;
}

void MeshesDrawer::InitResources()
{
    m_SkinnedShader = ResourceManager::Get<Shader>("skinned_gbuffer");
    constexpr ShaderProgramCullInfo cullInfo =
    {
        .enableCullFace = true,
        .cullFace = CullFace::Front,
        .frontFace = FrontFace::CW
    };

    m_SkinnedShader->SetFaceCullingInfo(cullInfo);
    m_SkinnedShader->CreateInInterface();
}

void MeshesDrawer::BeginFrame(const Scene& scene, const Renderer&)
{
    scene.GetAllComponentsOfType<SkinnedMeshRenderer>(&m_SkinnedRender);
    scene.GetAllComponentsOfType<StaticMeshRenderer>(&m_StaticMeshs);
    PrepareOctree(scene);
}

void MeshesDrawer::RenderAnimation() const
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

            for (uint32_t i = 0; i < skinnedMeshRender->mesh->models.GetSize(); i++)
            {
                const List<Matrix>& matrices = skinnedMeshRender->GetMatrices();
                for (size_t j = 0; j < matrices.GetSize(); j++)
                    m_SkinnedMeshGpuData->boneMatrices[j] = matrices[j];

                skinnedMeshRender->material.BindMaterial();
                Rhi::UpdateAnimationUniform(*m_SkinnedMeshGpuData);
                Rhi::DrawModel(DrawMode::Triangles, skinnedMeshRender->mesh->models[i]->GetId());
            }
        }
    }
    m_SkinnedShader->Unuse();
}

void MeshesDrawer::RenderAnimationNonShaded(const Scene& scene) const
{

    for (const SkinnedMeshRenderer* skinnedMeshRender : m_SkinnedRender)
    {
        ModelUniformData modelData;
        modelData.model = skinnedMeshRender->GetTransform().worldMatrix;
        modelData.meshRenderIndex = scene.GetEntityIndex(skinnedMeshRender->GetEntity()) + 1;

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
}


void MeshesDrawer::RenderStaticMesh(const MaterialType materialtype, const Camera& camera, const Frustum& frustum, const Scene& scene) const
{
    Rhi::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Fill);
    const OctreeIterator<OctreeNode<const StaticMeshRenderer>> it = scene.renderOctree.GetIterator();
    if (!camera.isOrthographic)
    {
#pragma region Draw OctreeFrustum

        while (true)
        {
            Bound bound = it.GetBound();
            // if we not see it 
            if (frustum.IsOnFrustum(bound))
            {
                std::vector<const StaticMeshRenderer*>* handle = nullptr;
                it.GetHandles(&handle);

                // draw
                for (const StaticMeshRenderer* const meshRenderer : *handle)
                {
                    for (size_t i = 0; i < meshRenderer->mesh->models.GetSize(); i++)
                    {
                        if (meshRenderer->material.materialType != materialtype )
                            continue;
                        
                        Pointer<Model> model = meshRenderer->mesh->models[i];
                        Bound aabb;
                        meshRenderer->GetAABB(&aabb);

                        if (!frustum.IsOnFrustum(aabb))
                        {
                            continue;
                        }

                        const Transform& transform = meshRenderer->GetEntity()->transform;
                        ModelUniformData modelData;
                        modelData.model = transform.worldMatrix;
                        // +1 to avoid the black color of the attachment be a valid index  
                            modelData.meshRenderIndex = scene.GetEntityIndex(meshRenderer->GetEntity()) + 1;

                        try
                        {
                            modelData.normalInvertMatrix = transform.worldMatrix.Inverted().Transposed();
                        }
                        catch (const std::invalid_argument&)
                        {
                            modelData.normalInvertMatrix = Matrix::Identity();
                        }
                        
                        if (model.IsValid())
                        {
                            meshRenderer->material.BindMaterial();
                            Rhi::UpdateModelUniform(modelData);
                            Rhi::DrawModel(DrawMode::Triangles, model->GetId());
                        }
                    }
                  
                }
            }
            if (!it.Iterate())
                break;
        }
#pragma endregion Draw OctreeFrustum
    }
    else
    {
#pragma region Draw iteration

        for (const StaticMeshRenderer* staticMeshRenderer : m_StaticMeshs)
        {
            if (!staticMeshRenderer->mesh)
                continue;

            for (size_t i = 0; i < staticMeshRenderer->mesh->models.GetSize(); i++)
            {
                Pointer<Model> model = staticMeshRenderer->mesh->models[i];
                const Transform& transform = staticMeshRenderer->GetEntity()->transform;
                ModelUniformData modelData;
                modelData.model = transform.worldMatrix;
                // +1 to avoid the black color of the attachment be a valid index  
                modelData.meshRenderIndex = scene.GetEntityIndex(staticMeshRenderer->GetEntity()) + 1;

                try
                {
                    modelData.normalInvertMatrix = transform.worldMatrix.Inverted().Transposed();
                }
                catch (const std::invalid_argument&)
                {
                    modelData.normalInvertMatrix = Matrix::Identity();
                }


                if (model.IsValid())
                {
                    staticMeshRenderer->material.BindMaterial();
                    Rhi::UpdateModelUniform(modelData);
                    Rhi::DrawModel(DrawMode::Triangles, model->GetId());
                }
            }
           
        }
#pragma endregion Draw iteration
    }
}

void MeshesDrawer::RenderStaticMeshNonShaded(const Camera& camera, const Frustum& frustum, const Scene& scene) const
{
    Rhi::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Fill);
    const OctreeIterator<OctreeNode<const StaticMeshRenderer>> it = scene.renderOctree.GetIterator();
    if (!camera.isOrthographic)
    {
#pragma region Draw OctreeFrustum

        while (true)
        {
            Bound bound = it.GetBound();
            // if we not see it 
            if (frustum.IsOnFrustum(bound))
            {
                std::vector<const StaticMeshRenderer*>* handle = nullptr;
                it.GetHandles(&handle);

                // draw
                for (const StaticMeshRenderer* const meshRenderer : *handle)
                {
                    for (size_t i = 0; i < meshRenderer->mesh->models.GetSize(); i++)
                    {
                        Pointer<Model> model = meshRenderer->mesh->models[i];
                        Bound aabb;
                        meshRenderer->GetAABB(&aabb);

                        if (!frustum.IsOnFrustum(aabb))
                        {
                            continue;
                        }

                        const Transform& transform = meshRenderer->GetEntity()->transform;
                        ModelUniformData modelData;
                        modelData.model = transform.worldMatrix;
                        // +1 to avoid the black color of the attachment be a valid index  
                            modelData.meshRenderIndex = scene.GetEntityIndex(meshRenderer->GetEntity()) + 1;

                        try
                        {
                            modelData.normalInvertMatrix = transform.worldMatrix.Inverted().Transposed();
                        }
                        catch (const std::invalid_argument&)
                        {
                            modelData.normalInvertMatrix = Matrix::Identity();
                        }
                        
                        if (model.IsValid())
                        {
                            Rhi::UpdateModelUniform(modelData);
                            Rhi::DrawModel(DrawMode::Triangles, model->GetId());
                        }
                    }
                  
                }
            }
            if (!it.Iterate())
                break;
        }
#pragma endregion Draw OctreeFrustum
    }
    else
    {
#pragma region Draw iteration

        for (const StaticMeshRenderer* mesh : m_StaticMeshs)
        {
            if (!mesh->mesh)
                continue;

            for (size_t i = 0; i < mesh->mesh->models.GetSize(); i++)
            {
                Pointer<Model> model = mesh->mesh->models[i];
                const Transform& transform = mesh->GetEntity()->transform;
                ModelUniformData modelData;
                modelData.model = transform.worldMatrix;
                // +1 to avoid the black color of the attachment be a valid index  
                modelData.meshRenderIndex = scene.GetEntityIndex(mesh->GetEntity()) + 1;

                try
                {
                    modelData.normalInvertMatrix = transform.worldMatrix.Inverted().Transposed();
                }
                catch (const std::invalid_argument&)
                {
                    modelData.normalInvertMatrix = Matrix::Identity();
                }


                if (model.IsValid())
                {
                    Rhi::UpdateModelUniform(modelData);
                    Rhi::DrawModel(DrawMode::Triangles, model->GetId());
                }
            }
           
        }
#pragma endregion Draw iteration
    }
}

void MeshesDrawer::PrepareOctree(const Scene& scene)
{
    std::vector<ObjectBounding<const StaticMeshRenderer>> meshrenderWithAabb;
	
    for (uint32_t i = 0; i < scene.GetEntities().GetSize();i++)
    {
        const Entity& ent = *scene.GetEntities()[i];

        const StaticMeshRenderer* meshRenderer = nullptr;
        if (ent.TryGetComponent(&meshRenderer))
        {
            if (!meshRenderer->mesh.IsValid())
                continue;

            Bound bound;
            meshRenderer->GetAABB(&bound);

            ObjectBounding<const StaticMeshRenderer> data;
            data.bound = bound;
            data.handle = meshRenderer;
            meshrenderWithAabb.emplace_back(data);
        }
    }
    scene.renderOctree.Update(meshrenderWithAabb);
}

void MeshesDrawer::EndFrame()
{
    // TO DO
    // In case of doing something at the end of frame 
}



