#include "rendering/renderer.hpp"

#include "rendering/rhi.hpp"
#include "resource/resource_manager.hpp"
#include "scene/component/mesh_renderer.hpp"

using namespace XnorCore;

void Renderer::RenderMenu(const Scene& scene)
{
    if (ImGui::BeginMenu("CurrentScene"))
    {
        if (ImGui::Checkbox("DrawScene Octree", &scene.renderOctree.draw))
        {
        }

        ImGui::EndMenu();
    }

    if (scene.renderOctree.draw)
    {
        scene.renderOctree.Draw();
    }
}

void Renderer::Initialize()
{
    Rhi::SetClearColor(clearColor);

    InitResources();
    m_SkyboxRenderer.InitializeResources();
    m_LightManager.InitResources();
    m_PostProcessPass.Init();
    m_AnimationRender.InitResources();
    Rhi::PrepareRendering();
}

void Renderer::BeginFrame(const Scene& scene, const Viewport& viewport)
{
    scene.GetAllComponentOfType<MeshRenderer>(&m_MeshRenderers);
    PrepareOctree(scene);
    m_LightManager.BeginFrame(scene, viewport, *this);
    m_AnimationRender.BeginFrame(scene, *this);
}

void Renderer::EndFrame(const Scene& scene)
{
    m_MeshRenderers.clear();
    m_LightManager.EndFrame(scene);
    m_AnimationRender.EndFrame();
}

void Renderer::RenderViewport(const Viewport& viewport, const Scene& scene)
{
    BeginFrame(scene,viewport);
    
	BindCamera(*viewport.camera,viewport.viewPortSize);
	m_Frustum.UpdateFromCamera(*viewport.camera,viewport.GetAspect());
	const ViewportData& viewportData = viewport.viewportData;
	DeferedRenderring(*viewport.camera, scene, viewportData, viewport.viewPortSize);
	ForwardPass(m_MeshRenderers, scene, viewport, viewport.viewPortSize, viewport.isEditor);
	
	if (viewportData.usePostProcess)
		m_PostProcessPass.Compute(*viewport.viewportData.colorAttachment , *viewport.m_Image, viewportData.postprocessRendertarget);


    EndFrame(scene);
}

void Renderer::ZPass(const Scene& scene, const Camera& camera,
                     const RenderPassBeginInfo& renderPassBeginInfo, const RenderPass& renderPass,
                     const Pointer<Shader>& shaderToUse,
                     bool_t drawEditorUi)
{
    scene.GetAllComponentOfType<MeshRenderer>(&m_MeshRenderers);
    RenderNonShadedPass(scene, camera, renderPassBeginInfo, renderPass, shaderToUse, drawEditorUi);
    m_MeshRenderers.clear();
}


void Renderer::SwapBuffers() const
{
    Rhi::SwapBuffers();
}



void Renderer::DeferedRenderring(const Camera& camera, const Scene& scene, const ViewportData& viewportData, const Vector2i viewportSize) const 
{
    const RenderPassBeginInfo renderPassBeginInfo =
    {
        .frameBuffer = viewportData.gFramebuffer,
        .renderAreaOffset = {0, 0},
        .renderAreaExtent = viewportSize,
        .clearBufferFlags = static_cast<BufferFlag::BufferFlag>(BufferFlag::ColorBit | BufferFlag::DepthBit),
        .clearColor = clearColor
    };

    viewportData.gBufferPass.BeginRenderPass(renderPassBeginInfo);

    // Draw Simple Mesh
    m_GBufferShader->Use();
    DrawMeshRendersByType(MaterialType::Opaque, scene);
    m_GBufferShader->Unuse();
    // DrawSkinnedMesh
    m_AnimationRender.RenderAnimation();

    viewportData.gBufferPass.EndRenderPass();

    const RenderPassBeginInfo renderPassBeginInfoLit =
    {
        .frameBuffer = viewportData.renderBuffer,
        .renderAreaOffset = {0, 0},
        .renderAreaExtent = viewportSize,
        .clearBufferFlags = BufferFlag::ColorBit,
        .clearColor = clearColor
    };


    viewportData.colorPass.BeginRenderPass(renderPassBeginInfoLit);
    m_GBufferShaderLit->Use();

    // Set G buffer Shader Info
    viewportData.BindDescriptor();
    scene.skybox.BindDesriptorSet();
    m_LightManager.BindShadowMap();

    Rhi::DrawModel(DrawMode::Triangles, m_Quad->GetId());

    scene.skybox.UnbindDesriptorSet();
    viewportData.UnbindDescriptor();
    viewportData.colorPass.EndRenderPass();

    m_GBufferShaderLit->Unuse();
}

void Renderer::ForwardPass(const std::vector<const MeshRenderer*>& meshRenderers, const Scene& scene,
                           const Viewport& viewport, const Vector2i viewportSize, const bool_t isEditor) const
{
    const ViewportData& viewportData = viewport.viewportData;

    const RenderPassBeginInfo renderPassBeginInfoLit =
    {
        .frameBuffer = viewportData.renderBuffer,
        .renderAreaOffset = {0, 0},
        .renderAreaExtent = viewportSize,
        .clearBufferFlags = BufferFlag::None,
        .clearColor = clearColor
    };

    viewportData.colorPass.BeginRenderPass(renderPassBeginInfoLit);

    m_Forward->Use();
    DrawMeshRendersByType(MaterialType::Lit, scene);
    m_Forward->Unuse();

    if (isEditor)
    {
        DrawAabb(meshRenderers);
    }

    m_SkyboxRenderer.DrawSkymap(m_Cube, scene.skybox);

    if (isEditor)
    {
        m_LightManager.DrawLightGizmo(*viewport.camera, *World::scene);
    }

    viewportData.colorPass.EndRenderPass();
}

void Renderer::DrawAabb(const std::vector<const MeshRenderer*>& meshRenderers) const
{
    m_GizmoShader->Use();
    Rhi::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Line);
    ModelUniformData modelData;
    m_GizmoShader->SetVec3("color", {0.f, 1.f, 0.f});

    for (const MeshRenderer* const meshRenderer : meshRenderers)
    {
        if (!meshRenderer->model.IsValid())
            continue;

        if (!meshRenderer->drawModelAabb)
            continue;

        const Transform& transform = meshRenderer->GetEntity()->transform;
        const Bound&& modelAabb = Bound::GetAabbFromTransform(meshRenderer->model->GetAabb(), transform);
        const Matrix&& trsAabb = Matrix::Trs(modelAabb.center, Quaternion::Identity(), modelAabb.extents);
        modelData.model = trsAabb;
        Rhi::UpdateModelUniform(modelData);

        Rhi::DrawModel(DrawMode::Triangles, m_Cube->GetId());
    }

    m_GizmoShader->Unuse();
    Rhi::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Fill);
}

void Renderer::PrepareOctree(const Scene& scene)
{
    std::vector<ObjectBounding<const MeshRenderer>> meshrenderWithAabb;

    for (uint32_t i = 0; i < m_MeshRenderers.size(); i++)
    {
        const MeshRenderer* meshRenderer = m_MeshRenderers[i];

        if (!meshRenderer->model.IsValid())
            continue;

        Bound bound = bound.GetAabbFromTransform(meshRenderer->model->GetAabb(), meshRenderer->entity->transform);
        ObjectBounding<const MeshRenderer> data;
        data.bound = bound;
        data.handle = meshRenderer;
        meshrenderWithAabb.emplace_back(data);
    }
    scene.renderOctree.Update(meshrenderWithAabb);
}


void Renderer::DrawMeshRendersByType(const MaterialType materialType, const Scene& scene) const
{
    Rhi::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Fill);


    const OctreeIterator<OctreeNode<const MeshRenderer>> it = scene.renderOctree.GetIterator();

    while (true)
    {
        Bound bound = it.GetBound();
        // if we not see it 
        if (m_Frustum.IsOnFrustum(bound))
        {
            std::vector<const MeshRenderer*>* handle = nullptr;
            it.GetHandles(&handle);

#pragma region Draw
            // draw
            for (const MeshRenderer* const meshRenderer : *handle)
            {
                if (meshRenderer->material.materialType != materialType)
                    continue;

                Bound aabb;
                meshRenderer->GetAABB(&aabb);

                if (!m_Frustum.IsOnFrustum(aabb))
                {
                    continue;
                }

                const Transform& transform = meshRenderer->GetEntity()->transform;
                ModelUniformData modelData;
                modelData.model = transform.worldMatrix;

                try
                {
                    modelData.normalInvertMatrix = transform.worldMatrix.Inverted().Transposed();
                }
                catch (const std::invalid_argument&)
                {
                    modelData.normalInvertMatrix = Matrix::Identity();
                }


                if (meshRenderer->model.IsValid())
                {
                    Rhi::UpdateModelUniform(modelData);
                    meshRenderer->material.BindMaterial();
                    Rhi::DrawModel(DrawMode::Triangles, meshRenderer->model->GetId());
                }
#pragma endregion Draw
            }
        }
        if (!it.Iterate())
            break;
    }
}


void Renderer::DrawAllMeshRendersNonShaded(const Camera& camera, const Scene& scene) const
{
    Rhi::SetPolygonMode(PolygonFace::FrontAndBack, PolygonMode::Fill);
    const OctreeIterator<OctreeNode<const MeshRenderer>> it = scene.renderOctree.GetIterator();
    if (!camera.isOrthographic)
    {
#pragma region Draw OctreeFrustum

        while (true)
        {
            Bound bound = it.GetBound();
            // if we not see it 
            if (m_Frustum.IsOnFrustum(bound))
            {
                std::vector<const MeshRenderer*>* handle = nullptr;
                it.GetHandles(&handle);

                // draw
                for (const MeshRenderer* const meshRenderer : *handle)
                {
                    Bound aabb;
                    meshRenderer->GetAABB(&aabb);

                    if (!m_Frustum.IsOnFrustum(aabb))
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


                    if (meshRenderer->model.IsValid())
                    {
                        Rhi::UpdateModelUniform(modelData);
                        Rhi::DrawModel(DrawMode::Triangles, meshRenderer->model->GetId());
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

        for (const MeshRenderer* mesh : m_MeshRenderers)
        {
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


            if (mesh->model.IsValid())
            {
                Rhi::UpdateModelUniform(modelData);
                Rhi::DrawModel(DrawMode::Triangles, mesh->model->GetId());
            }
        }
#pragma endregion Draw iteration
    }

    
}

void Renderer::RenderNonShadedPass(const Scene& scene, const Camera& camera,
                                   const RenderPassBeginInfo& renderPassBeginInfo,
                                   const RenderPass& renderPass, const Pointer<Shader>& shaderToUse,
                                   bool_t drawEditorUi)
{
    const Vector2i viewportSize = renderPassBeginInfo.renderAreaOffset + renderPassBeginInfo.renderAreaExtent;
    BindCamera(camera, viewportSize);
    m_Frustum.UpdateFromCamera(camera, static_cast<float_t>(viewportSize.x) / static_cast<float_t>(viewportSize.y));
    renderPass.BeginRenderPass(renderPassBeginInfo);
    DrawAllMeshRendersNonShaded(camera, scene);

    if (drawEditorUi)
    {
        m_LightManager.DrawLightGizmoWithShader(camera, scene, shaderToUse);
    }

    renderPass.EndRenderPass();
}

void Renderer::BindCamera(const Camera& camera, const Vector2i screenSize) const
{
    CameraUniformData cam;
    camera.GetView(&cam.view);
    camera.GetProjection(screenSize, &cam.projection);

    cam.cameraPos = camera.position;
    cam.near = camera.near;
    cam.far = camera.far;
    Rhi::UpdateCameraUniform(cam);
}

void Renderer::InitResources()
{
    // Deferred 
    m_GBufferShaderLit = ResourceManager::Get<Shader>("deferred_opaque");
    m_GBufferShaderLit->SetDepthFunction(DepthFunction::Disable);

    m_GBufferShaderLit->CreateInInterface();
    m_GBufferShaderLit->Use();

    m_GBufferShaderLit->SetInt("gPosition", DefferedDescriptor::Position);
    m_GBufferShaderLit->SetInt("gNormal", DefferedDescriptor::Normal);
    m_GBufferShaderLit->SetInt("gAlbedoSpec", DefferedDescriptor::Albedo);
    m_GBufferShaderLit->SetInt("gMetallicRoughessReflectance", DefferedDescriptor::MetallicRoughessReflectance);
    m_GBufferShaderLit->SetInt("gAmbiantOcclusion", DefferedDescriptor::AmbiantOcclusion);
    m_GBufferShaderLit->SetInt("gEmissive", DefferedDescriptor::Emissivive);

    m_GBufferShaderLit->SetInt("irradianceMap", DefferedDescriptor::SkyboxIrradiance);
    m_GBufferShaderLit->SetInt("prefilterMap", DefferedDescriptor::SkyboxPrefilterMap);
    m_GBufferShaderLit->SetInt("brdfLUT", DefferedDescriptor::SkyboxPrecomputeBrdf);

    m_GBufferShaderLit->SetInt("dirLightShadowMap", ShadowTextureBinding::Directional);
    m_GBufferShaderLit->SetInt("spotLightShadowArray", ShadowTextureBinding::SpotLight);
    m_GBufferShaderLit->SetInt("pointLightCubemapArrayPixelDistance",
                               ShadowTextureBinding::PointLightCubemapArrayPixelDistance);

    m_GBufferShaderLit->Unuse();

    m_GBufferShader = ResourceManager::Get<Shader>("gbuffer");

    constexpr ShaderProgramCullInfo cullInfo =
    {
        .enableCullFace = true,
        .cullFace = CullFace::Front,
        .frontFace = FrontFace::CW
    };

    m_GBufferShader->SetFaceCullingInfo(cullInfo);
    m_GBufferShader->CreateInInterface();

    // Init diffuse Texture for gbuffer
    m_GBufferShader->Use();
    m_GBufferShader->SetInt("material.albedoMap", MaterialTextureEnum::Albedo);
    m_GBufferShader->SetInt("material.metallicMap", MaterialTextureEnum::Metallic);
    m_GBufferShader->SetInt("material.roughnessMap", MaterialTextureEnum::Roughness);
    m_GBufferShader->SetInt("material.normalMap", MaterialTextureEnum::Normal);
    m_GBufferShader->SetInt("material.ambiantOcclusionMap", MaterialTextureEnum::AmbiantOcclusion);
    m_GBufferShader->Unuse();
    // End deferred

    // Forward
    m_Forward = ResourceManager::Get<Shader>("basic_shader");
    m_Forward->CreateInInterface();

    m_DrawTextureToScreenShader = ResourceManager::Get<Shader>("draw_texture_to_screen");
    m_DrawTextureToScreenShader->CreateInInterface();

    m_GizmoShader = ResourceManager::Get<Shader>("gizmo_shader");
    m_GizmoShader->CreateInInterface();

    m_DrawTextureToScreenShader->Use();
    m_DrawTextureToScreenShader->SetInt("bufferTextureId", 0);
    m_DrawTextureToScreenShader->Unuse();
    // End forward

    // Primitive
    m_Cube = ResourceManager::Get<Model>("assets/models/cube.obj");
    m_Quad = ResourceManager::Get<Model>("assets/models/quad.obj");
}
