#pragma once

#include "core.hpp"

#include "Maths/vector2i.hpp"
#include "Maths/vector3.hpp"
#include "Maths/matrix.hpp"

BEGIN_XNOR_CORE

class XNOR_ENGINE Camera
{
public:
	Vector3 pos;
	float near = 0.1f;
	Vector3 front = Vector3::UnitZ();
	float far = 1000.f;
	Vector3 up = Vector3::UnitY();
	float fov = 90.f;
	Vector3 right = Vector3::UnitX();
	Vector2i screenSize = {800,600};

	void GetProjection(Matrix* matrix) const;

	void GetView(Matrix* matrix) const;

	void GetVp(Matrix* matrix) const;

	[[nodiscard]]
	Vector2i ProjectOn(const Vector3& vertex, const Matrix& model) const;
};

END_XNOR_CORE
