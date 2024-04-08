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

<<<<<<< HEAD
bool_t Frustum::AABBCollidWithPlane(const Plane& plane, const Bound& aabb, const Vector3& center) const
=======
bool_t Frustum::AABBCollidWithPlane(const Plane& plane, const Model::Aabb&, const Vector3& center) const
>>>>>>> dev
{
    const float r = center.x * std::abs(plane.normal.x) +
            center.y * std::abs(plane.normal.y) + center.z * std::abs(plane.normal.z);

    return -r <= plane.GetSignedDistanceToPlane(center);
}

<<<<<<< HEAD
bool_t Frustum::IsInFrutum(const Camera& camera, const Bound& aabb, const XnorCore::Transform& transform) const
=======
bool_t Frustum::IsInFrutum(const Camera&, const Model::Aabb& aabb, const Transform& transform) const
>>>>>>> dev
{
    const Vector3&& right = transform.GetRight() * aabb.extents.x;
    const Vector3&& up = transform.GetUp() * aabb.extents.y;
    const Vector3&& forward = transform.GetForward() * aabb.extents.z;

    const float_t newIi = Calc::Abs(Vector3::Dot(Vector3::UnitX(), right)) +
      Calc::Abs(Vector3::Dot(Vector3::UnitX(), up)) +
      Calc::Abs(Vector3::Dot(Vector3::UnitX(), forward));

    const float_t newIj = Calc::Abs(Vector3::Dot(Vector3::UnitY(), right)) +
       Calc::Abs(Vector3::Dot(Vector3::UnitY(), up)) +
       Calc::Abs(Vector3::Dot(Vector3::UnitY(), forward));

    const float_t newIk = Calc::Abs(Vector3::Dot(Vector3::UnitZ(), right)) +
       Calc::Abs(Vector3::Dot(Vector3::UnitZ(), up)) +
       Calc::Abs(Vector3::Dot(Vector3::UnitZ(), forward));

    /*
    const Aabb obb =
    {
        .min = static_cast<Vector3>(transform.worldMatrix * Vector4(aabb.min.x, aabb.min.y , aabb.min.z, 1.f)),
        .max = static_cast<Vector3>(transform.worldMatrix * Vector4(aabb.max.x, aabb.max.y , aabb.max.z, 1.f)),
    };
*/
    
    
    
    

    return false;
}
