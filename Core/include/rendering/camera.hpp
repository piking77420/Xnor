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
	float_t near = 0.1f;
	Vector3 front = Vector3::UnitZ();
	float_t far = 1000.f;
	Vector3 up = Vector3::UnitY();
	float_t fov = 90.f;
	Vector3 right = Vector3::UnitX();

	void GetProjection(const Vector2i screenSize,Matrix* matrix) const;

	void GetView(Matrix* matrix) const;

	void GetVp(const Vector2i screenSize,Matrix* matrix) const;

	[[nodiscard]]
	Vector2i ProjectOn(const Vector3& vertex,const Vector2i screenSize, const Matrix& model) const;
	
};

END_XNOR_CORE
