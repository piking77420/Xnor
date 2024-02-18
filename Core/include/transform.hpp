#pragma once

#include "Maths/vector3.hpp"
#include "Maths/quaternion.hpp"

#include "core.hpp"
#include "utils/reflectable.hpp"

BEGIN_XNOR_CORE

class Transform final : public Reflectable
{
	REFLECTABLE_IMPL(Transform)
	
public:
	Vector3 position = 0.f;
	Quaternion rotation = Quaternion::Identity();
	Vector3 scale = 1.f;
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::Transform),
	field(position),
	field(rotation),
	field(scale)
)
