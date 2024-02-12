#pragma once

#include "core.hpp"

#include "Maths/vector2i.hpp"
#include "Maths/vector3.hpp"
#include "Maths/matrix.hpp"

BEGIN_XNOR_CORE

class XNOR_ENGINE Camera
{
public:


	Vector3 pos = 0.f;
	float near = 0.1f;
	Vector3 front = Vector3::UnitZ();
	float far = 1000.f;
	Vector3 up = Vector3::UnitY();
	float fov = 90.f;
	Vector2i screenSize = 1000;

	void GetProjection(Matrix* matrix);

	void GetView(Matrix* matrix);

	void GetVP(Matrix* matrix);

	Vector2i ProjectOn(const Vector3& vertex, const Matrix& model);


};

END_XNOR_CORE
