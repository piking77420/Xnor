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
    
    explicit PickingStrategy(Editor* editor);

    ~PickingStrategy();
    
    // Should be call on the Imgui editor window only
    void ResizeHandle(Vector2i newSize);

    // Pixel pos in image current Window
    bool GetEntityFromScreen(Vector2i pixelPos, XnorCore::Scene& scene,const XnorCore::Camera& pointOfView,
    XnorCore::Entity** entity);
private:
    Editor* m_Editor = nullptr;
    XnorCore::Pointer<XnorCore::Shader> m_PickingShader;
    
    XnorCore::RenderPass m_ColorPass;
    XnorCore::FrameBuffer* frameBuffer = nullptr;

    XnorCore::Texture* m_ColorAttachment = nullptr;
    XnorCore::Texture* m_DepthAttachement = nullptr;
    
    void DestroyRendering() const;

    void InitRendering(Vector2i size);
};

END_XNOR_EDITOR
