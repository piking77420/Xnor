#include "rendering/frustum.hpp"

#include "resource/model.hpp"

using namespace XnorCore;

Plane::Plane(const Vector3 p1, const Vector3 newNormal) : normal(newNormal.Normalized()),distance(Vector3::Dot(normal,p1))
{
    
}

float_t Plane::GetSignedDistanceToPlane(const Vector3& point) const
{
    return Vector3::Dot(normal, point) - distance;
}


void Frustum::UpdateFromCamera(const Camera& camera, const float_t aspect)
{
    if (camera.isOrthographic)
        UpdateCameraOrthoGraphic(camera, aspect);
    else
        UpdateCameraPerspective(camera, aspect);
        
    
}

bool_t Frustum::IsOnFrustum(const Bound& bound) const
{
    bool_t top  = bound.IsOnPlane(plane[Top]);
    bool_t bottom  = bound.IsOnPlane(plane[Bottom]);
    bool_t near  = bound.IsOnPlane(plane[Near]);
    bool_t far  = bound.IsOnPlane(plane[Far]);
    bool_t right  = bound.IsOnPlane(plane[Right]);
    bool_t left  = bound.IsOnPlane(plane[Left]);

    return top && bottom && near && far && right && left;
}

void Frustum::UpdateCameraPerspective(const Camera& camera, float_t aspect)
{
    const float_t halfVSide = camera.far * tanf(camera.fov * Calc::Deg2Rad * .5f);
    const float_t halfHSide = halfVSide * aspect;
    
    const Vector3 frontMultFar = camera.far * camera.front;
    
    plane[Near] = Plane(camera.position + camera.near * camera.front, camera.front);
    
    plane[Far] = Plane(camera.position + frontMultFar, -camera.front);
    
    plane[Right] = Plane(camera.position,Vector3::Cross(frontMultFar - camera.right * halfHSide, camera.up));
    
    plane[Left] = Plane(camera.position,Vector3::Cross(camera.up,frontMultFar + (camera.right * halfHSide)));
    
    plane[Top] = Plane(camera.position, Vector3::Cross(camera.right, frontMultFar - (camera.up * halfVSide)));
    
    plane[Bottom] = Plane(camera.position, Vector3::Cross(frontMultFar + (camera.up * halfVSide), camera.right));

}   

void Frustum::UpdateCameraOrthoGraphic(const Camera& camera, float_t aspect)
{
    const float_t halfVSide = camera.far * 0.5f;
    const float_t halfHSide = halfVSide * aspect;

    const Vector3 frontMultNear = camera.near * camera.front;
    const Vector3 frontMultFar = camera.far * camera.front;

    plane[Near] = { camera.position + frontMultNear, camera.front };
    plane[Far] = { camera.position + frontMultFar, -camera.front };
    plane[Right] = { camera.position + camera.right * halfHSide, camera.right };
    plane[Left] = { camera.position - camera.right * halfHSide, -camera.right };
    plane[Top] = { camera.position + camera.up * halfVSide, camera.up };
    plane[Bottom] = { camera.position - camera.up * halfVSide, -camera.up };
}
