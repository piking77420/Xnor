#include "transform.hpp"

Vector3 XnorCore::Transform::GetWorldPos()
{
    return static_cast<Vector3>(worldMatrix[3]);
}
