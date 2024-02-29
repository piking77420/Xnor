#include "rendering/renderer_context.hpp"

using namespace XnorCore;

bool RendererContext::IsValid() const
{
    return camera != nullptr &&  framebuffer != nullptr;  
}
