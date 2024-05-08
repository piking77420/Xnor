#include "rendering/camera.hpp"

#include <Maths/matrix.hpp>

using namespace XnorCore;

void Camera::GetProjection(const Vector2i screenSize, Matrix* const matrix) const
{
	if (!isOrthographic)
	{
		const float_t ratio = static_cast<float_t>(screenSize.x) / static_cast<float_t>(screenSize.y);
		//Matrix::Perspective(fov * Calc::Deg2Rad, ratio, near, far, matrix);
		*matrix = Perspective(fov * Calc::Deg2Rad , ratio , near, far);
		return;
	}
	Matrix::Orthographic(leftRight.x, leftRight.y, bottomtop.x, bottomtop.y, near ,far, matrix);
}

void Camera::GetView(Matrix* const matrix) const
{
	*matrix = LookAtRH(position, position + front, up);
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
	front = (at - position).Normalized();
	right = Vector3::Cross(Vector3::UnitY(),front).Normalized();
	up = Vector3::Cross(front ,right).Normalized();
}

void Camera::LookAt(const Vector3& at, const Vector3& upVector)
{
	front = (at - position).Normalized();
	right = Vector3::Cross(upVector,front).Normalized();
	up = Vector3::Cross(front ,right).Normalized();
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

Matrix Camera::Perspective(const float_t fovy, const float_t aspect, const float_t zNear, const float_t zFar)
{
	float_t const tanHalfFovy = tanf(fovy / 2.f);

	Matrix result;
	result.m00 = 1.f / (aspect * tanHalfFovy);
	result.m11 = 1.f / (tanHalfFovy);
	result.m22 = -(zFar + zNear) / (zFar - zNear);
	result.m32 = -1.f;
	result.m23 = -(2.f * zFar * zNear) / (zFar - zNear);
	return result;
}

Matrix Camera::LookAtRH(Vector3 const& eye, Vector3 const& center, Vector3 const& up)
{
	Vector3 const f((center - eye).Normalized());
	Vector3 const s(Vector3::Cross(f, up).Normalized());
	Vector3 const u(Vector3::Cross(s, f));

	Matrix m = Matrix::Identity();
	m.m00 = s.x;
	m.m01 = s.y;
	m.m02 = s.z;
	m.m10 = u.x;
	m.m11 = u.y;
	m.m12 = u.z;
	m.m20 = -f.x;
	m.m21 = -f.y;
	m.m22 = -f.z;
	m.m03 = -Vector3::Dot(s, eye);
	m.m13 = -Vector3::Dot(u, eye);
	m.m23 =  Vector3::Dot(f, eye);
	
	return m;
}
