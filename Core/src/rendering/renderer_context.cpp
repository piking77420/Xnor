#include "rendering/renderer_context.hpp"

using namespace XnorCore;

bool_t RendererContext::IsValid() const
{
    return camera != nullptr &&  frameBuffer != nullptr;  
}
