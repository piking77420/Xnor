#pragma once

#include "Maths/vector3.hpp"
#include "Maths/quaternion.hpp"

class Transform
{
public:
	Vector3 position = 0.f;
	Quaternion rotation = Quaternion::Identity();
	Vector3 scale = 1.f;
};

