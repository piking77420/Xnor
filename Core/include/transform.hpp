#pragma once

#include "Maths/quaternion.hpp"
#include "Maths/vector3.hpp"

#include "core.hpp"
#include "utils/reflectable.hpp"
#include "serialization/serializer.hpp"

BEGIN_XNOR_CORE

class Transform final : public Reflectable
{
private:
	REFLECTABLE_IMPL(Transform)

public:
	Vector3 position;
	Vector3 rotation; 
	
	Vector3 scale = Vector3(1.f);
	Quaternion quaternion = Quaternion::Identity();
};

END_XNOR_CORE

REFL_AUTO(type(XnorCore::Transform),
	field(position),
	field(rotation),
	field(scale)
)
