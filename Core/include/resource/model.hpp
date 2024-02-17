#pragma once

#include <assimp/mesh.h>

#include "core.hpp"
#include "resource.hpp"
#include "rendering/vertex.hpp"

BEGIN_XNOR_CORE

class Model : public Resource
{
public:
    // Use the base class' constructors
    using Resource::Resource;
    
    // Use the defined destructor from the base class instead of the deleted one from the interface
    //using Resource::~Resource;

    // We keep both function overloads and only override one
    using Resource::Load;
    
    XNOR_ENGINE void Load(const uint8_t* buffer, int64_t length) override;
    
    XNOR_ENGINE void Load(const aiMesh& loadedData);
    
    XNOR_ENGINE void Unload() override;

    uint32_t GetID() const;
    
private:
    std::vector<Vertex> m_Vertices;
    std::vector<uint32_t> m_Indices;
    uint32_t m_ModelId = 0;
};

END_XNOR_CORE
