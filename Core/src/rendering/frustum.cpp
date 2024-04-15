#include "rendering/frustum.hpp"

#include "resource/model.hpp"

using namespace XnorCore;

Plane::Plane(Vector3 p1, Vector3 normal) : normal(p1.Normalized()),distance(Vector3::Dot(normal,p1))
{
    
}

float_t Plane::GetSignedDistanceToPlane(const Vector3& point) const
{
    return Vector3::Dot(normal, point) - distance;
}


void Frustum::UpdateFromCamera(const Camera& camera, const float_t aspect)
{
    const float_t halfVSide = camera.far * tanf(camera.fov * Calc::Deg2Rad * .5f);
    const float_t halfHSide = halfVSide * aspect;
    
    const Vector3 frontMultNear = camera.near * camera.front;
    const Vector3 frontMultFar = camera.far * camera.front;
    
    plane[Near] = { camera.position + frontMultNear, camera.front };
    plane[Far] = { camera.position + frontMultFar, -camera.front };
    plane[Right] = { camera.position,Vector3::Cross(frontMultFar - camera.right * halfHSide, camera.up) };
    plane[Left] = { camera.position,Vector3::Cross(camera.up,frontMultFar + (camera.right * halfHSide)) };
    plane[Top] = { camera.position, Vector3::Cross(camera.right, frontMultFar - (camera.up * halfVSide)) };
    plane[Bottom] = { camera.position, Vector3::Cross(frontMultFar + (camera.up * halfVSide), camera.right) };

}

bool_t Frustum::IsOnFrustum(const Bound& bound) const
{
    return bound.IsOnPlane(plane[0]) && bound.IsOnPlane(plane[1]) && bound.IsOnPlane(plane[2]) && bound.IsOnPlane(plane[3]) && bound.IsOnPlane(plane[4]);
}
