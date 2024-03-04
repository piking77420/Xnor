#include "transform.hpp"
#include "serialization/serializer.hpp"

using namespace XnorCore;

REFLECTABLE_IMPL_CPP(Transform)

Vector3 Transform::GetWorldPos() const
{
    return static_cast<Vector3>(worldMatrix[3]);
}
