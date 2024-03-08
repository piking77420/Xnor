#pragma once

#include <array>

#include "core.hpp"
#include "resource/resource.hpp"

BEGIN_XNOR_CORE

/// @brief Holds the necessary information to draw text using a Font.
class Font : public Resource
{
public:
    /// @brief Allowed extensions for font files
    XNOR_ENGINE static inline constexpr std::array<const char_t*, 1> FileExtensions
    {
        ".ttf"
    };
};

END_XNOR_CORE
