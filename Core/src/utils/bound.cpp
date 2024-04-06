#include "utils/bound.hpp"

using namespace XnorCore;

Bound::Bound(const Vector3 minimum, const Vector3 maximum) : min(minimum),max(maximum) , size((max - min)) ,
center((max + min) * 0.5f)
{
    
}

Bound::Bound(const Vector3 newCenter, const float_t extendX, const float_t extendY, const float_t extendZ) : size( Vector3(extendX, extendY, extendZ)),
center(newCenter)
{
    const Vector3 sizeHalf = size * 0.5f;
    max = (center + sizeHalf);
    min = (center - sizeHalf);
}

Bound Bound::GetAabbFromTransform(const Transform& transform)
{
    
    const Vector3 returnCenter = static_cast<Vector3>(transform.worldMatrix * Vector4(center.x, center.y, center.z, 1.f));
    const Vector3 right = static_cast<Vector3>(transform.worldMatrix[0]) * size.x;
    const Vector3 up =  static_cast<Vector3>(transform.worldMatrix[1]) * size.y;
    const Vector3 forward =  static_cast<Vector3>(transform.worldMatrix[2]) * size.z;

    const float_t newExtendX = std::abs(Vector3::Dot(Vector3::UnitX(),right))  + 
        std::abs(Vector3::Dot(Vector3::UnitX(),up))  +  std::abs(Vector3::Dot(Vector3::UnitX(),forward));

    const float_t newExtendY = std::abs(Vector3::Dot(Vector3::UnitY(),right))  + 
       std::abs(Vector3::Dot(Vector3::UnitY(),up))  +  std::abs(Vector3::Dot(Vector3::UnitY(),forward));

    const float_t newExtendZ = std::abs(Vector3::Dot(Vector3::UnitZ(),right))  + 
       std::abs(Vector3::Dot(Vector3::UnitZ(),up))  +  std::abs(Vector3::Dot(Vector3::UnitZ(),forward));

    // Let the constructor
    return Bound(returnCenter, newExtendX, newExtendY, newExtendZ);
}

bool_t Bound::Intersect(const Bound& otherBound) const
{
    if (otherBound.min.x <= min.x || otherBound.min.y <= min.y  || otherBound.min.z <= min.z)
        return true;
    
    if (otherBound.max.x >= max.x || otherBound.max.y >= max.y  || otherBound.max.z >= max.z)
        return true;

    return false;
}

void Bound::Encapsulate(const Bound& encapsulateBound)
{
    if (encapsulateBound.min.x < min.x)
        min.x = encapsulateBound.min.x;
    if (encapsulateBound.min.y < min.y)
        min.y = encapsulateBound.min.y;
    if (encapsulateBound.min.z < min.z)
        min.z = encapsulateBound.min.z;

    if (encapsulateBound.max.x > max.x)
        max.x = encapsulateBound.max.x;
    if (encapsulateBound.max.y > max.y)
        max.y = encapsulateBound.max.y;
    if (encapsulateBound.max.z > max.z)
        max.z = encapsulateBound.max.z;

    size = ((max - min));
    center = ((max + min));
}
