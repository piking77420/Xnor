#pragma once

#include "core.hpp"
#include "resource.hpp"
#include "rendering/vertex.hpp"

BEGIN_XNOR_CORE

class Model : public Resource
{
public:
    using Resource::Resource;

    using Resource::Load;
    
    XNOR_ENGINE void Load(const uint8_t* buffer, int64_t length) override;
    
    XNOR_ENGINE void Unload() override;

private:
    std::vector<Vertex> m_Vertices;
    std::vector<uint32_t> m_Indices;
    uint32_t m_ModelId = 0;
};

END_XNOR_CORE
