#include "camera.hpp"

using namespace XnorCore;



void Camera::GetProjection(Matrix* matrix)
{
	float ratio = static_cast<float>(screenSize.x) / static_cast<float>(screenSize.y);
	Matrix::Perspective(fov * Calc::Deg2Rad, ratio, near, far, *matrix);
}

void Camera::GetView(Matrix* matrix)
{
	Matrix::LookAt(pos,pos+ front,up,*matrix);
}

void Camera::GetVP(Matrix* matrix)
{
	Matrix view;
	Matrix projection;

	GetView(&view);
	GetProjection(&projection);
	*matrix = projection * view;
}

Vector2i Camera::ProjectOn(const Vector3& vertex,const Matrix& model)
{

	Matrix MVP;
	GetVP(&MVP);
	MVP *= model;

	Vector4&& vec = MVP * Vector4(vertex.x, vertex.y, vertex.z, vertex.y);

	float divide = 1.f / vec.w;

	vec.x *= divide;
	vec.y *= divide;
	vec.z *= divide;

	return Vector2i(vec.x, vec.y);
}
