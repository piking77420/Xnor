#pragma once

#include <array>

#include <assimp/mesh.h>

#include "core.hpp"
#include "resource.hpp"
#include "rendering/vertex.hpp"

BEGIN_XNOR_CORE

// ReSharper disable once CppInconsistentNaming
struct ModelAABB
{
    Vector3 min { std::numeric_limits<float_t>::max() };
    Vector3 max { std::numeric_limits<float_t>::min() };
};

class Model : public Resource
{
public:
    XNOR_ENGINE static inline constexpr std::array<const char_t*, 58> FileExtensions
    {
        ".3d",
        ".3ds",
        ".3mf",
        ".ac",
        ".ac3d",
        ".acc",
        ".amj",
        ".ase",
        ".ask",
        ".b3d",
        ".bvh",
        ".csm",
        ".cob",
        ".dae",
        ".dxf",
        ".enff",
        ".fbx",
        ".gltf",
        ".glb",
        ".hmb",
        ".ifc",
        ".irr",
        ".irrmesh",
        ".lwo",
        ".lws",
        ".lxo",
        ".m3d",
        ".md2",
        ".md3",
        ".md5",
        ".mdc",
        ".mdl",
        ".mesh",
        ".mot",
        ".ms3d",
        ".ndo",
        ".nff",
        ".obj",
        ".off",
        ".ogex",
        ".ply",
        ".pmx",
        ".prj",
        ".q3o",
        ".q3s",
        ".raw",
        ".scn",
        ".sib",
        ".smd",
        ".stp",
        ".stl",
        ".ter",
        ".uc",
        ".vta",
        ".x",
        ".x3d",
        ".xgl",
        ".zgl"
    };
    
    // Use the base class' constructors
    using Resource::Resource;

    // We keep both function overloads and only override one
    using Resource::Load;

    DEFAULT_COPY_MOVE_OPERATIONS(Model)

    XNOR_ENGINE ~Model() override;
    
    XNOR_ENGINE void Load(const uint8_t* buffer, int64_t length) override;
    
    XNOR_ENGINE void Load(const aiMesh& loadedData);

    XNOR_ENGINE void CreateInRhi() override;

    XNOR_ENGINE void DestroyInRhi() override;
    
    XNOR_ENGINE void Unload() override;

    [[nodiscard]]
    XNOR_ENGINE uint32_t GetId() const;
    
private:
    std::vector<Vertex> m_Vertices;
    std::vector<uint32_t> m_Indices;
    uint32_t m_ModelId = 0;
    
    ModelAABB m_Aabb;
};

END_XNOR_CORE
