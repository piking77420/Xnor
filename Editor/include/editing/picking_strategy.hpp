#pragma once

#include "definitions.hpp"
#include "rendering/camera.hpp"
#include "rendering/render_pass.hpp"
#include "resource/shader.hpp"
#include "scene/entity.hpp"
#include "world/skybox.hpp"


BEGIN_XNOR_EDITOR
    class Editor;
class Shader;

class PickingStrategy
{
public:
    XnorCore::FrameBuffer* frameBuffer = nullptr;
    
    explicit PickingStrategy(Editor* editor);
    
    // Should be call on the Imgui editor window only
    void Resize(Vector2i newSize);

    // Pixel pos in image current Window
    bool GetEntityFromScreen(Vector2i pixelPos, XnorCore::Scene& scene,const XnorCore::Camera& pointOfView,
    XnorCore::Entity** entity);
private:
    Editor* m_Editor = nullptr;
    XnorCore::Pointer<XnorCore::Shader> m_PickingShader;


    void DestroyRendering() const;

    void InitRendering(Vector2i size);
    
    XnorCore::RenderPass m_ColorPass;
    XnorCore::Texture* colorAttachment = nullptr;
    XnorCore::Texture* m_DepthAttachement = nullptr;
};

END_XNOR_EDITOR
