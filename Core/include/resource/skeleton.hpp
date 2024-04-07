#pragma once

#include <array>

#include <assimp/mesh.h>

#include "core.hpp"
#include "file/file.hpp"
#include "rendering/bone.hpp"
#include "resource/resource.hpp"
#include "utils/list.hpp"

BEGIN_XNOR_CORE

class Skeleton : public Resource
{
public:    
    /// @brief Allowed extensions for skeletons.
    XNOR_ENGINE static inline constexpr std::array<const char_t*, 3> FileExtensions
    {
        ".max",
        ".xsi",
        ".ma"
        // ".fbx"
    };

    // Use the base class' constructors
    using Resource::Resource;

    // We keep both function overloads and only override one
    using Resource::Load;
    
    DEFAULT_COPY_MOVE_OPERATIONS(Skeleton)

    XNOR_ENGINE ~Skeleton() override = default;

    /// @copydoc XnorCore::Resource::Load(const uint8_t* buffer, int64_t length)
    XNOR_ENGINE bool_t Load(const uint8_t* buffer, int64_t length) override;

    /// @brief Loads a Model from assimp loaded data.
    XNOR_ENGINE bool_t Load(const aiSkeleton& loadedData);

private:
    List<Bone> m_Bones;

    void ComputeGlobalMatrices();
};

END_XNOR_CORE
