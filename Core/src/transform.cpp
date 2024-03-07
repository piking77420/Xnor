#include "transform.hpp"
#include "serialization/serializer.hpp"

using namespace XnorCore;

REFLECTABLE_IMPL_CPP(Transform)

Vector3& Transform::SetPosition()
{
    m_Changed = true;
    return m_Position;
}

Vector3& Transform::SetRotationEulerAngle()
{
    m_Changed = true;
    return m_EulerRotation;
}

Quaternion& Transform::SetRotation()
{
    m_Changed = true;
    return m_Rotation;
}

Vector3& Transform::SetScale()
{
    m_Changed = true;
    return m_Scale;
}

const Vector3& Transform::GetPosition() const
{
    return m_Position;
}

const Vector3& Transform::GetRotationEulerAngle() const
{
    return m_EulerRotation;
}

const Quaternion& Transform::GetRotation() const
{
    return m_Rotation;
}

const Vector3& Transform::GetScale() const
{
    return m_Scale;
}

bool_t Transform::HasChanged() const
{
    return m_Changed;
}