#include "utils/bound.hpp"

using namespace XnorCore;

Bound:: Bound(const Vector3 newCenter, const Vector3 newSize) : size(newSize),
center(newCenter)
{
    const Vector3 sizeHalf = size * 0.5f;
    max = (center + sizeHalf);
    min = (center - sizeHalf);
}

void Bound::SetMinMax(Vector3 newmin, Vector3 newMax)
{
    max = newMax;
    min = newmin;
    center = (max + min) * 0.5f;
    size = ((max - min));
}

Bound Bound::GetAabbFromTransform(const Bound& bound,const Transform& transform)
{
    
    const Vector3 returnCenter = static_cast<Vector3>(transform.worldMatrix * Vector4(bound.center.x, bound.center.y, bound.center.z, 1.f));
    const Vector3 right = static_cast<Vector3>(transform.worldMatrix[0]) * bound.size.x * 0.5f;
    const Vector3 up =  static_cast<Vector3>(transform.worldMatrix[1]) * bound.size.y * 0.5f;
    const Vector3 forward =  static_cast<Vector3>(transform.worldMatrix[2]) * bound.size.z * 0.5f;

    const float_t newExtendX = std::abs(Vector3::Dot(Vector3::UnitX(),right))  + 
        std::abs(Vector3::Dot(Vector3::UnitX(),up))  +  std::abs(Vector3::Dot(Vector3::UnitX(),forward));

    const float_t newExtendY = std::abs(Vector3::Dot(Vector3::UnitY(),right))  + 
       std::abs(Vector3::Dot(Vector3::UnitY(),up))  +  std::abs(Vector3::Dot(Vector3::UnitY(),forward));

    const float_t newExtendZ = std::abs(Vector3::Dot(Vector3::UnitZ(),right))  + 
       std::abs(Vector3::Dot(Vector3::UnitZ(),up))  +  std::abs(Vector3::Dot(Vector3::UnitZ(),forward));

    // Let the constructor
    return Bound(returnCenter, Vector3(newExtendX,newExtendY,newExtendZ) * 2.f);
}

bool Bound::Intersect(const Bound& otherBound)
{
    return !(otherBound.max.x < min.x || otherBound.min.x > max.x ||
             otherBound.max.y < min.y || otherBound.min.y > max.y ||
             otherBound.max.z < min.z || otherBound.min.z > max.z);
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
    center = ((max + min) * 0.5f);
}
