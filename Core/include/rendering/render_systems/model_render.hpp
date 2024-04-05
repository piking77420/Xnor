#pragma once

#include <map>

#include "core.hpp"
#include "transform.hpp"
#include "Maths/matrix.hpp"
#include "rendering/camera.hpp"
#include "rendering/material.hpp"
#include "scene/component/mesh_renderer.hpp"

BEGIN_XNOR_CORE
    class ModelRender
{
public:
    struct MeshRenderRequest
    {
        bool_t isInstanced = false;
        uint32_t entityIndex;
        const MeshRenderer* meshRenderer = nullptr;
    };
    
   XNOR_ENGINE void DrawModel(const MeshRenderRequest& meshRenderRequest);
    
    
private:
    
    struct ModelInstancing
    {
        Material material;
        std::vector<Matrix> modelsMatrix;
        std::vector<Matrix> modelsInvertedNormal;
    };

    // TODO WRAPER FOR VAO VBO EVO
    struct ModelInternal
    {
        uint32_t vao = 0;
        uint32_t vbo = 0;
        uint32_t ebo = 0;
        uint32_t nbrOfVertex = 0;
        uint32_t nbrOfIndicies = 0;
    };
    
    XNOR_ENGINE static inline std::map<uint32_t,ModelInstancing> m_ModelInstancingMap;
};

END_XNOR_CORE
