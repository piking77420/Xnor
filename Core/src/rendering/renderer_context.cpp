#include "rendering/renderer_context.hpp"

using namespace XnorCore;

bool RendererContext::IsValid()
{
    return camera != nullptr &&  framebuffer != nullptr;  
}
