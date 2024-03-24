#include "rendering/postprocess_rendertarget.hpp"

using namespace XnorCore;


void PostprocessRendertarget::Init(Vector2i viewPortsize)
{
    bloomRenderTarget.Initialize(viewPortsize);
}

void PostprocessRendertarget::Destroy()
{
    bloomRenderTarget.Destroy();
}

void PostprocessRendertarget::Resize(Vector2i viewPortsize)
{
    bloomRenderTarget.Resize(viewPortsize);
}
