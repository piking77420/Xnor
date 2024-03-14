#include "rendering/render_systems/equirectangular_to_cubemap.hpp"

#include "utils/logger.hpp"

using namespace XnorCore;

void EquirectangularToCubeMap::Initalize()
{
    
}

void EquirectangularToCubeMap::Shutdown()
{
    
}

void EquirectangularToCubeMap::Compute(const Texture& equirectangularMap, const Cubemap& cubemap)
{
#ifdef _DEBUG
    if (m_FrameBuffer == nullptr)
    {
        Logger::LogFatal("Equirectangular  class has not been initialize");
        throw std::runtime_error("Equirectangular  class has not been initialize");
    }
#endif

    
    
    
}
