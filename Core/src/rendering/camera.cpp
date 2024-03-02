#include "rendering/camera.hpp"

#include <Maths/matrix.hpp>

using namespace XnorCore;

void Camera::GetProjection(const Vector2i screenSize, Matrix* const matrix) const
{
	const float_t ratio = static_cast<float_t>(screenSize.x) / static_cast<float_t>(screenSize.y);
	Matrix::Perspective(fov * Calc::Deg2Rad, ratio, near, far, matrix);
}

void Camera::GetView(Matrix* const matrix) const
{
	Matrix::LookAt(pos, pos + front, up, matrix);
}

void Camera::GetVp(const Vector2i screenSize, Matrix* matrix) const
{
	Matrix view;
	Matrix projection;

	GetView(&view);
	GetProjection(screenSize, &projection);
	*matrix = projection * view;
}

void Camera::LookAt(const Vector3& at)
{
	front = -(at - pos).Normalized();
	right = Vector3::Cross(Vector3::UnitY(),front).Normalized();
	up = Vector3::Cross(front,right).Normalized();
}

Vector2i Camera::ProjectOn(const Vector3& vertex, const Vector2i screenSize, const Matrix& model) const
{
	Matrix mvp;
	GetVp(screenSize,&mvp);
	mvp *= model;

	Vector4 vec = mvp * Vector4(vertex.x, vertex.y, vertex.z, vertex.y);

	const float_t divide = 1.f / vec.w;

	vec.x *= divide;
	vec.y *= divide;
	vec.z *= divide;

	return Vector2i(static_cast<int32_t>(vec.x), static_cast<int32_t>(vec.y));
}
