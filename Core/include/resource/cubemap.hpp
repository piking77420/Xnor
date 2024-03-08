#pragma once

#include <array>
#include <Maths/vector2i.hpp>

#include "core.hpp"
#include "resource.hpp"

/// @file cubemap.hpp
/// @brief Defines the XnorCore::Cubemap class

BEGIN_XNOR_CORE

/// @brief Represents an array of 6 @ref Texture "Textures"
class Cubemap : public Resource
{
public:
    /// @brief Creates a cubemap using 6 provided texture file names
    /// @param cubeMapsTextures Texture file names
    XNOR_ENGINE explicit Cubemap(const std::array<std::string, 6>& cubeMapsTextures);

    /// @brief Creates the Cubemap in the Rhi
    XNOR_ENGINE void CreateInRhi() override;

    /// @brief Destroys the Cubemap in the Rhi
    XNOR_ENGINE void DestroyInRhi() override;

    /// @brief Unloads the Cubemap
    XNOR_ENGINE void Unload() override;

    /// @brief Binds the Cubemap
    XNOR_ENGINE void BindTexture(uint32_t unit) const;

    /// @brief Unbinds the Cubemap
    XNOR_ENGINE void UnBindTexture(uint32_t unit) const;
    
private:
    uint32_t m_Id = 0;
    std::array<void*, 6> m_Images;
    Vector2i m_CubemapSize;
    int32_t m_DataChannels = 0;
};

END_XNOR_CORE
