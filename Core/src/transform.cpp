#include "transform.hpp"

using namespace XnorCore;

const Vector3& Transform::GetPosition() const
{
    return m_Position;
}

void Transform::SetPosition(const Vector3& newPosition)
{
    m_Changed = true;
    m_Position = newPosition;
}

void Transform::SetPositionX(const float_t newPositionX)
{
    m_Changed = true;
    m_Position.x = newPositionX;
}

void Transform::SetPositionY(const float_t newPositionY)
{
    m_Changed = true;
    m_Position.y = newPositionY;
}

void Transform::SetPositionZ(const float_t newPositionZ)
{
    m_Changed = true;
    m_Position.z = newPositionZ;
}

const Vector3& Transform::GetRotationEulerAngle() const
{
    return m_EulerRotation;
}

void Transform::SetRotationEulerAngle(const Vector3& newRotationEulerAngle)
{
    m_Changed = true;
    m_EulerRotation = newRotationEulerAngle;
}

void Transform::SetRotationEulerAngleX(const float_t newRotationEulerAngleX)
{
    m_Changed = true;
    m_EulerRotation.x = newRotationEulerAngleX;
}

void Transform::SetRotationEulerAngleY(const float_t newRotationEulerAngleY)
{
    m_Changed = true;
    m_EulerRotation.y = newRotationEulerAngleY;
}

void Transform::SetRotationEulerAngleZ(const float_t newRotationEulerAngleZ)
{
    m_Changed = true;
    m_EulerRotation.z = newRotationEulerAngleZ;
}

const Quaternion& Transform::GetRotation() const
{
    return m_Rotation;
}

void Transform::SetRotation(const Quaternion& newRotation)
{
    m_Changed = true;
    m_Rotation = newRotation;
}

void Transform::SetRotationX(const float_t newRotationX)
{
    m_Changed = true;
    m_Rotation.X() = newRotationX;
}

void Transform::SetRotationY(const float_t newRotationY)
{
    m_Changed = true;
    m_Rotation.Y() = newRotationY;
}

void Transform::SetRotationZ(const float_t newRotationZ)
{
    m_Changed = true;
    m_Rotation.Z() = newRotationZ;
}

void Transform::SetRotationW(const float_t newRotationW)
{
    m_Changed = true;
    m_Rotation.W() = newRotationW;
}

const Vector3& Transform::GetScale() const
{
    return m_Scale;
}

void Transform::SetScale(const Vector3& newScale)
{
    m_Changed = true;
    m_Scale = newScale;
}

void Transform::SetScaleX(const float_t newScaleX)
{
    m_Changed = true;
    m_Scale.x = newScaleX;
}

void Transform::SetScaleY(const float_t newScaleY)
{
    m_Changed = true;
    m_Scale.y = newScaleY;
}

void Transform::SetScaleZ(const float_t newScaleZ)
{
    m_Changed = true;
    m_Scale.z = newScaleZ;
}

bool_t Transform::GetChanged() const
{
    return m_Changed;
}

Vector3 Transform::GetRight() const
{
    return static_cast<Vector3>(worldMatrix * Vector4::UnitX());
}

Vector3 Transform::GetUp() const
{
    return static_cast<Vector3>(worldMatrix * Vector4::UnitY());
}

Vector3 Transform::GetForward() const
{
    return static_cast<Vector3>(worldMatrix * Vector4::UnitZ());
}
