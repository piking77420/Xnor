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

    // We keep both function overloads and only override one
    using Resource::Load;

    ~Model();
    
    XNOR_ENGINE void Load(const uint8_t* buffer, int64_t length) override;
    
    XNOR_ENGINE void Load(const aiMesh& loadedData);
    
    XNOR_ENGINE void Unload() override;

    [[nodiscard]]
    uint32_t GetId() const;
    
private:
    std::vector<Vertex> m_Vertices;
    std::vector<uint32_t> m_Indices;
    uint32_t m_ModelId = 0;
};

END_XNOR_CORE
