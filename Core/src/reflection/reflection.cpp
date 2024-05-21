#include "reflection/reflection.hpp"
#include "reflection/xnor_factory.hpp"

using namespace XnorCore;

void Reflection::CloneUsingFactory(const void* const src, void* const dst, const size_t hash)
{
    XnorFactory::CloneObject(src, dst, hash);
}

void* Reflection::CreateUsingFactory(const size_t hash)
{
    return XnorFactory::CreateObject(hash);
}
