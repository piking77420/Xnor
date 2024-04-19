#pragma once

#include <array>

#include <assimp/mesh.h>
#include <assimp/scene.h>

#include "core.hpp"
#include "resource/animation.hpp"
#include "resource/model.hpp"
#include "skeleton.hpp"
#include "texture.hpp"
#include "rendering/material.hpp"
#include "resource/resource.hpp"
#include "utils/list.hpp"

BEGIN_XNOR_CORE

class Mesh final : public Resource
{
    REFLECTABLE_IMPL(Mesh)
    
public:
    /// @brief Allowed extensions for meshes.
    XNOR_ENGINE static constexpr std::array<const char_t*, 2> FileExtensions
    {
        ".fbx",
        ".dae"
    };

    List<Model*> models;
    List<Texture*> textures;

    Material material;

    // Use the base class' constructors
    using Resource::Resource;

    // We keep both function overloads and only override one
    using Resource::Load;
    
    DEFAULT_COPY_MOVE_OPERATIONS(Mesh)

    XNOR_ENGINE ~Mesh() override;

    /// @copydoc XnorCore::Resource::Load(const uint8_t* buffer, int64_t length)
    XNOR_ENGINE bool_t Load(const uint8_t* buffer, int64_t length) override;

    /// @copydoc XnorCore::Resource::CreateInInterface()
    XNOR_ENGINE void CreateInInterface() override;

    XNOR_ENGINE Animation* GetAnimation(size_t id);

private:
    List<Animation*> m_Animations;
    List<Skeleton*> m_Skeletons;
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::Mesh, bases<XnorCore::Resource>),
    field(material),
    field(m_Animations)
)
