#pragma once
#include "rendering/camera.hpp"
#include "frame_buffer.hpp"

BEGIN_XNOR_CORE

class RendererContext
{
public:
	const Camera* camera;
	const FrameBuffer* framebuffer;
	const RenderPass* renderPass;
	bool IsEditor = false;
};

END_XNOR_CORE
