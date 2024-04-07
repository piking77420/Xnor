#include "utils/bound.hpp"

using namespace XnorCore;

Bound:: Bound(const Vector3 newCenter, const Vector3 newSize) : extents(newSize * 0.5f),
center(newCenter)
{
}

Vector3 Bound::GetMin() const
{
    return center - extents;
}

Vector3 Bound::GetMax() const
{
    return center + extents;
}

Vector3 Bound::GetSize() const
{
    return extents * 2.f; 
}

void Bound::SetMinMax(Vector3 newmin, Vector3 newMax)
{
    extents = (newMax - newmin) * 0.5f;
    center = newmin + extents;
}

Bound Bound::GetAabbFromTransform(const Bound& bound,const Transform& transform)
{
    const Vector3 globalPos = static_cast<Vector3>(transform.worldMatrix * Vector4(bound.center.x, bound.center.y, bound.center.z, 1.f));
    // Let the constructor
    return ReturnAabbFromMatrix(bound, transform.worldMatrix, globalPos);
}

Bound Bound::GetAabbFromCamera(const Bound& bound, const Camera& cam)
{
    Matrix view;
    cam.GetView(&view);
    
    return ReturnAabbFromMatrix(bound, view, cam.position);
}

bool Bound::Intersect(const Bound& otherBound) const
{
    const Vector3 max = GetMax();
    const Vector3 min = GetMin();

    const Vector3 otherMax = otherBound.GetMax();
    const Vector3 otherMin = otherBound.GetMin();
    
    return min.x <= otherMax.x && max.x >= otherMin.x && min.y <= otherMax.y && max.y >= otherMin.y && min.z <= otherMax.z && max.z >= otherMin.z;
}

void Bound::Encapsulate(const Bound& encapsulateBound)
{
    Encapsulate(encapsulateBound.center - encapsulateBound.extents);
    Encapsulate(encapsulateBound.center + encapsulateBound.extents);
}

void Bound::Encapsulate(Vector3 point)
{
    const Vector3 min = GetMin();
    const Vector3 max = GetMax();
    
    const float_t minX = std::min(min.x, point.x);
    const float_t minY = std::min(min.y, point.y);
    const float_t minZ = std::min(min.z, point.z);
    
    const float_t maxX = std::max(max.x, point.x);
    const float_t maxY = std::max(max.y, point.y);
    const float_t maxZ = std::max(max.z, point.z);

    SetMinMax( { minX, minY, minZ }, { maxX, maxY, maxZ });
}

Bound Bound::ReturnAabbFromMatrix(const Bound& bound,const Matrix& matrix, const Vector3 center)
{
    const Vector3 right = static_cast<Vector3>(matrix[0]) * bound.extents.x;
    const Vector3 up =  static_cast<Vector3>(matrix[1]) * bound.extents.y;
    const Vector3 forward =  static_cast<Vector3>(matrix[2]) * bound.extents.z;
    
    const float_t newExtendX = std::abs(Vector3::Dot(Vector3::UnitX(),right))  + 
       std::abs(Vector3::Dot(Vector3::UnitX(),up))  +  std::abs(Vector3::Dot(Vector3::UnitX(),forward));

    const float_t newExtendY = std::abs(Vector3::Dot(Vector3::UnitY(),right))  + 
       std::abs(Vector3::Dot(Vector3::UnitY(),up))  +  std::abs(Vector3::Dot(Vector3::UnitY(),forward));

    const float_t newExtendZ = std::abs(Vector3::Dot(Vector3::UnitZ(),right))  + 
       std::abs(Vector3::Dot(Vector3::UnitZ(),up))  +  std::abs(Vector3::Dot(Vector3::UnitZ(),forward));

    return Bound(center, Vector3(newExtendX,newExtendY,newExtendZ) * 2.f);
}
