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
    const float halfVSide = camera.far * tanf(camera.fov * Calc::Deg2Rad * .5f);
    const float halfHSide = halfVSide * aspect;
    const Vector3 frontMultFar = camera.far * camera.front;
    
    m_Plane[Near] = { camera.position + camera.near * camera.front, camera.front };
    m_Plane[Far] = { camera.position + frontMultFar, -camera.front };
    m_Plane[Right] = { camera.position,Vector3::Cross(frontMultFar - camera.right * halfHSide, camera.up) };
    m_Plane[Left] = { camera.position,Vector3::Cross(camera.up,frontMultFar + camera.right * halfHSide) };
    m_Plane[Top] = { camera.position, Vector3::Cross(camera.right, frontMultFar - camera.up * halfVSide) };
    m_Plane[Bottom] = { camera.position, Vector3::Cross(frontMultFar + camera.up * halfVSide, camera.right) };

}

bool_t Frustum::AbbCollidWithPlane(const Plane& plane, const Model::Aabb& aabb, const Vector3& center) const
{
    const float r = center.x * std::abs(plane.normal.x) +
            center.y * std::abs(plane.normal.y) + center.z * std::abs(plane.normal.z);

    return -r <= plane.GetSignedDistanceToPlane(center);
}



bool Frustum::IsInFrutum(const Camera& camera, const Model::Aabb& aabb, const XnorCore::Transform& transform) const
{
    const Vector3 pos = static_cast<Vector3>(transform.worldMatrix[3]);

    const Model::Aabb obb =
    {
        .min = static_cast<Vector3>(transform.worldMatrix * Vector4(aabb.min.x, aabb.min.y , aabb.min.z, 1.f)),
        .max = static_cast<Vector3>(transform.worldMatrix * Vector4(aabb.max.x, aabb.max.y , aabb.max.z, 1.f)),
    };

    for
    
}
