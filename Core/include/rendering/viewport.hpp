#pragma once

#include "frame_buffer.hpp"
#include "viewport_data.hpp"
#include "rendering/camera.hpp"
#include "render_systems/post_process_pass.hpp"
#include "render_systems/tone_mapping.hpp"
#include "scene/entity.hpp"

/// @file renderer_context.hpp
/// @brief Defines the XnorCore::RendererContext class.

BEGIN_XNOR_CORE

/// @brief Class used to store everything needed to render a frame.
///
/// Stores the main @ref XnorCore::Viewport::camera "camera" along with the @ref XnorCore::Viewport::frameBuffer "frameBuffer".
class Viewport
{
public:
	
	
	/// @brief The Camera used to render a frame.
	const Camera* camera = nullptr;

	Vector2i viewPortSize = Window::GetSize();

	RenderPass colorPass;
	
	FrameBuffer* frameBuffer = nullptr;
	
	/// @brief Whether this is used in the editor.
	bool_t isEditor = false;
	
	ViewportData viewportData;

	XNOR_ENGINE void Init(Vector2i newSize);

	XNOR_ENGINE void Destroy();

	XNOR_ENGINE void OnResize(Vector2i newSize);
	
	// image to give to renderer
	XNOR_ENGINE Texture* GetImage() const;
	/// @brief Whether the context is valid.
	[[nodiscard]]
	XNOR_ENGINE bool_t IsValid() const;
	
private:
	Texture* m_Image = nullptr;
};

END_XNOR_CORE
