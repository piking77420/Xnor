#include "rendering/camera.hpp"

using namespace XnorCore;

void Camera::GetProjection(Matrix* matrix) const
{
	const float ratio = static_cast<float>(screenSize.x) / static_cast<float>(screenSize.y);
	Matrix::Perspective(fov * Calc::Deg2Rad, ratio, near, far, *matrix);
}

void Camera::GetView(Matrix* matrix) const
{
	Matrix::LookAt(pos, pos + front, up, *matrix);
}

void Camera::GetVp(Matrix* matrix) const
{
	Matrix view;
	Matrix projection;

	GetView(&view);
	GetProjection(&projection);
	*matrix = projection * view;
}

Vector2i Camera::ProjectOn(const Vector3& vertex, const Matrix& model) const
{
	Matrix mvp;
	GetVp(&mvp);
	mvp *= model;

	Vector4 vec = mvp * Vector4(vertex.x, vertex.y, vertex.z, vertex.y);

	const float divide = 1.f / vec.w;

	vec.x *= divide;
	vec.y *= divide;
	vec.z *= divide;

	return Vector2i(static_cast<int>(vec.x), static_cast<int>(vec.y));
}
