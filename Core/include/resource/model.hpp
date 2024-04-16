#pragma once

#include <array>

#include <assimp/mesh.h>

#include "core.hpp"
#include "file/file.hpp"
#include "rendering/vertex.hpp"
#include "resource/resource.hpp"

/// @file model.hpp
/// @brief Defines the XnorCore::Model class.

BEGIN_XNOR_CORE

/// @brief Holds the necessary information to draw a 3D model.
class Model final : public Resource
{
public:
    /// @brief Struct used to store the minimum and maximum bounds of the AABB of a Model.
    struct Aabb
    {
        /// @brief The minimum bound of this AABB.
        Vector3 min { std::numeric_limits<float_t>::max() };
        /// @brief The maximum bound of this AABB.
        Vector3 max { std::numeric_limits<float_t>::min() };
    };

    /// @brief Allowed extensions for models.
    XNOR_ENGINE static inline constexpr std::array<const char_t*, 56> FileExtensions
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
        ".dxf",
        ".enff",
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

    /// @brief Destroys this Model.
    XNOR_ENGINE ~Model() override;

    /// @copydoc XnorCore::Resource::Load(const uint8_t* buffer, int64_t length)
    XNOR_ENGINE bool_t Load(const uint8_t* buffer, int64_t length) override;

    /// @brief Loads a Model from assimp loaded data.
    XNOR_ENGINE bool_t Load(const aiMesh& loadedData);

    /// @copydoc XnorCore::Resource::CreateInRhi
    XNOR_ENGINE void CreateInRhi() override;

    /// @copydoc XnorCore::Resource::DestroyInRhi
    XNOR_ENGINE void DestroyInRhi() override;
    
    /// @copydoc XnorCore::Resource::Unload
    XNOR_ENGINE void Unload() override;

    /// @brief Gets the id of the model
    /// @return Model id
    [[nodiscard]]
    XNOR_ENGINE uint32_t GetId() const;

    /// @brief Gets the Aabb bounding box of the model
    /// @return Aabb bounding box
    XNOR_ENGINE Aabb GetAabb() const;

#ifndef SWIG
    /// @brief Gets the vertices of the model
    /// @return Vertices
    [[nodiscard]]
    const std::vector<Vertex>& GetVertices() const;
#endif
    
private:
    XNOR_ENGINE void ComputeAabb(const aiAABB& assimpAabb);
    
    std::vector<Vertex> m_Vertices;
    std::vector<uint32_t> m_Indices;
    uint32_t m_ModelId = 0;
    
    Aabb m_Aabb;
};

END_XNOR_CORE
