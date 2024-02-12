#pragma once

#include "Maths/vector3.hpp"
#include "Maths/quaternion.hpp"

#include "core.hpp"

BEGIN_XNOR_CORE

class Transform
{
public:
	Vector3 position = 0.f;
	Quaternion rotation = Quaternion::Identity();
	Vector3 scale = 1.f;
};

END_XNOR_CORE
