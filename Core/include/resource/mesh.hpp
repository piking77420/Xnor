#pragma once

#include <array>

#include <assimp/mesh.h>

#include "animation.hpp"
#include "core.hpp"
#include "model.hpp"
#include "skeleton.hpp"
#include "texture.hpp"
#include "assimp/scene.h"
#include "file/file.hpp"
#include "resource/resource.hpp"
#include "utils/list.hpp"

BEGIN_XNOR_CORE

class Mesh : public Resource
{
public:
    /// @brief Allowed extensions for meshes.
    XNOR_ENGINE static inline constexpr std::array<const char_t*, 1> FileExtensions
    {
        ".fbx"
    };

    List<Model*> models;
    
    // Use the base class' constructors
    using Resource::Resource;

    // We keep both function overloads and only override one
    using Resource::Load;
    
    DEFAULT_COPY_MOVE_OPERATIONS(Mesh)

    XNOR_ENGINE ~Mesh() override;

    /// @copydoc XnorCore::Resource::Load(const uint8_t* buffer, int64_t length)
    XNOR_ENGINE bool_t Load(const uint8_t* buffer, int64_t length) override;

private:
    List<Texture*> m_Textures;
    List<Skeleton*> m_Skeletons;
    List<Animation*> m_Animations;
};

END_XNOR_CORE
