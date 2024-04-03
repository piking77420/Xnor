#include "reflection/type_renderer.hpp"
#include "reflection/xnor_factory.hpp"

using namespace XnorCore;

void TypeRenderer::DisplayObjectUsingFactory(void* const obj, const size_t hash)
{
    XnorFactory::DisplayObject(obj, hash);
}
