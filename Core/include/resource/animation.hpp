#pragma once

#include <array>

#include "core.hpp"
#include "skeleton.hpp"
#include "assimp/scene.h"
#include "file/file.hpp"
#include "rendering/bone.hpp"
#include "resource/resource.hpp"
#include "utils/list.hpp"

BEGIN_XNOR_CORE

class Animation final : public Resource
{
public:
    /// @brief Allowed extensions for animations.
    XNOR_ENGINE static inline constexpr std::array<const char_t*, 0> FileExtensions
    {
    };

    // Use the base class' constructors
    using Resource::Resource;

    // We keep both function overloads and only override one
    using Resource::Load;
    
    DEFAULT_COPY_MOVE_OPERATIONS(Animation)

    XNOR_ENGINE ~Animation() override = default;

    XNOR_ENGINE void Update();

    XNOR_ENGINE void BindSkeleton(Skeleton* skeleton);

    /// @copydoc XnorCore::Resource::Load(const uint8_t* buffer, int64_t length)
    XNOR_ENGINE bool_t Load(const uint8_t* buffer, int64_t length) override;

    XNOR_ENGINE bool_t Load(const aiAnimation& loadedData);

    [[nodiscard]]
    XNOR_ENGINE const List<Matrix>& GetMatrices() const;

private:
    struct KeyFrame
    {
        Vector3 translation;
        Quaternion rotation;
        Vector3 scaling;
        float_t time;
    };
    
    double_t m_Duration;
    double_t m_Framerate;
    int32_t m_FrameCount;

    Skeleton* m_Skeleton;

    float_t m_Time;

    std::unordered_map<std::string, List<KeyFrame>> m_KeyFrames;

    List<Matrix> m_CurrentFrameMatrices;
};

END_XNOR_CORE
