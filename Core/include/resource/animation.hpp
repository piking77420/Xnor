#pragma once

#include <array>

#include "core.hpp"
#include "assimp/scene.h"
#include "file/file.hpp"
#include "rendering/bone.hpp"
#include "resource/resource.hpp"
#include "utils/list.hpp"

BEGIN_XNOR_CORE

class Animation : public Resource
{
public:
    /// @brief Allowed extensions for skeletons.
    XNOR_ENGINE static inline constexpr std::array<const char_t*, 0> FileExtensions
    {
    };

    // Use the base class' constructors
    using Resource::Resource;

    // We keep both function overloads and only override one
    using Resource::Load;
    
    DEFAULT_COPY_MOVE_OPERATIONS(Animation)

    XNOR_ENGINE ~Animation() override = default;

    /// @copydoc XnorCore::Resource::Load(const uint8_t* buffer, int64_t length)
    XNOR_ENGINE bool_t Load(const uint8_t* buffer, int64_t length) override;

    XNOR_ENGINE bool_t Load(const aiAnimation& loadedData);

private:
    struct KeyFrame
    {
        Vector3 translation;
        Quaternion rotation;
        Vector3 scaling;
    };
    
    double_t m_Duration;
    double_t m_Framerate;

    std::vector<std::vector<KeyFrame>> m_KeyFrames;
};

END_XNOR_CORE
