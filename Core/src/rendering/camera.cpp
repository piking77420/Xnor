#include "rendering/camera.hpp"

#include <Maths/matrix.hpp>

using namespace XnorCore;

void Camera::GetProjection(const Vector2i screenSize, Matrix* const matrix) const
{
	if (!isOrthographic)
	{
		const float_t ratio = static_cast<float_t>(screenSize.x) / static_cast<float_t>(screenSize.y);
		Matrix::Perspective(fov * Calc::Deg2Rad, ratio, near, far, matrix);
		return;
	}
	Matrix::Orthographic(leftRight.x, leftRight.y, bottomtop.x, bottomtop.y, near ,far, matrix);
}

void Camera::GetView(Matrix* const matrix) const
{
	Matrix::LookAt(position, position + front, up, matrix);
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
	front = (position - at).Normalized();
	right = Vector3::Cross(front, Vector3::UnitY()).Normalized();
	up = Vector3::Cross(right, front).Normalized();
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
	// vec.z *= divide;

	return Vector2i(static_cast<int32_t>(vec.x), static_cast<int32_t>(vec.y));
}

bool_t Camera::IsOnFrustum(const Bound& bound,const Vector2i screeSize) const
{
	Frustum frustum;
	frustum.UpdateFromCamera(*this,static_cast<float_t>(screeSize.x) / static_cast<float_t>(screeSize.y));
	
	return bound.IsOnPlane(frustum.plane[0]) && bound.IsOnPlane(frustum.plane[1]) && bound.IsOnPlane(frustum.plane[2]) && bound.IsOnPlane(frustum.plane[3]) && bound.IsOnPlane(frustum.plane[4]);
}
