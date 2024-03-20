#include "transform.hpp"

using namespace XnorCore;

const Vector3& Transform::GetPosition() const
{
    return m_Position;
}

Vector3& Transform::SetPosition()
{
    m_Changed = true;
    return m_Position;
}

const Vector3& Transform::GetRotationEulerAngle() const
{
    return m_EulerRotation;
}

Vector3& Transform::SetRotationEulerAngle()
{
    m_Changed = true;
    return m_EulerRotation;
}

const Quaternion& Transform::GetRotation() const
{
    return m_Rotation;
}

Quaternion& Transform::SetRotation()
{
    m_Changed = true;
    return m_Rotation;
}

const Vector3& Transform::GetScale() const
{
    return m_Scale;
}

Vector3& Transform::SetScale()
{
    m_Changed = true;
    return m_Scale;
}

bool_t Transform::GetChanged() const
{
    return m_Changed;
}
