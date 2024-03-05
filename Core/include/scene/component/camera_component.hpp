#pragma once

#include "core.hpp"
#include "rendering/camera.hpp"
#include "scene/component.hpp"

/// @file camera_component.hpp
/// @brief Defines the CameraComponent class.

BEGIN_XNOR_CORE

/// @brief Component that encapsulates a @ref Camera
class XNOR_ENGINE CameraComponent : public Component
{
public:
	/// @brief Camera
	Camera camera;
};

END_XNOR_CORE
