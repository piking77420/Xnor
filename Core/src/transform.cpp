#include "transform.hpp"

#include "world/scene_graph.hpp"

using namespace XnorCore;

Vector3 Transform::GetPosition() const
{
    return static_cast<Vector3>(worldMatrix[3]);
}

void Transform::SetPosition(const Vector3& newPosition)
{
    m_Position = newPosition;
    SceneGraph::UpdateTransform(*entity);
}

void Transform::SetPositionX(const float_t newPositionX)
{
    m_Position.x = newPositionX;
    SceneGraph::UpdateTransform(*entity);
}

void Transform::SetPositionY(const float_t newPositionY)
{
    m_Position.y = newPositionY;
    SceneGraph::UpdateTransform(*entity);
}

void Transform::SetPositionZ(const float_t newPositionZ)
{
    m_Position.z = newPositionZ;
    SceneGraph::UpdateTransform(*entity);
}

const Vector3& Transform::GetRotationEulerAngle() const
{
    return m_EulerRotation;
}

void Transform::SetRotationEulerAngle(const Vector3& newRotationEulerAngle)
{
    m_EulerRotation = newRotationEulerAngle;
    m_Rotation = Quaternion::FromEuler(m_EulerRotation).Normalized();
}

void Transform::SetRotationEulerAngleX(const float_t newRotationEulerAngleX)
{
    m_EulerRotation.x = newRotationEulerAngleX;
    m_Rotation = Quaternion::FromEuler(m_EulerRotation).Normalized();
}

void Transform::SetRotationEulerAngleY(const float_t newRotationEulerAngleY)
{
    m_EulerRotation.y = newRotationEulerAngleY;
    m_Rotation = Quaternion::FromEuler(m_EulerRotation).Normalized();
}

void Transform::SetRotationEulerAngleZ(const float_t newRotationEulerAngleZ)
{
    m_EulerRotation.z = newRotationEulerAngleZ;
    SceneGraph::UpdateTransform(*entity);
}

const Quaternion& Transform::GetRotation() const
{
    return m_Rotation;
}

void Transform::SetRotation(const Quaternion& newRotation)
{
    m_Rotation = newRotation;
    m_EulerRotation = Quaternion::ToEuler(m_Rotation);
    SceneGraph::UpdateTransform(*entity);
}

void Transform::SetRotationX(const float_t newRotationX)
{
    m_Rotation.X() = newRotationX;
    m_EulerRotation = Quaternion::ToEuler(m_Rotation);
    SceneGraph::UpdateTransform(*entity);
}

void Transform::SetRotationY(const float_t newRotationY)
{
    m_Rotation.Y() = newRotationY;
    m_EulerRotation = Quaternion::ToEuler(m_Rotation);
    SceneGraph::UpdateTransform(*entity);
}

void Transform::SetRotationZ(const float_t newRotationZ)
{
    m_Rotation.Z() = newRotationZ;
    m_EulerRotation = Quaternion::ToEuler(m_Rotation);
    SceneGraph::UpdateTransform(*entity);
}

void Transform::SetRotationW(const float_t newRotationW)
{
    m_Rotation.W() = newRotationW;
    SceneGraph::UpdateTransform(*entity);
}


void Transform::SetScale(const Vector3& newScale)
{
    m_Scale = newScale;
    SceneGraph::UpdateTransform(*entity);
}

void Transform::SetScaleX(const float_t newScaleX)
{
    m_Scale.x = newScaleX;
    SceneGraph::UpdateTransform(*entity);
}

void Transform::SetScaleY(const float_t newScaleY)
{
    m_Scale.y = newScaleY;
    SceneGraph::UpdateTransform(*entity);
}

void Transform::SetScaleZ(const float_t newScaleZ)
{
    m_Scale.z = newScaleZ;
    SceneGraph::UpdateTransform(*entity);
}

Vector3 Transform::GetRight() const
{
    return (Matrix3(worldMatrix) * Vector3::UnitX()).Normalized();
}

Vector3 Transform::GetUp() const
{
    return (Matrix3(worldMatrix) * Vector3::UnitY()).Normalized();
}

Vector3 Transform::GetForward() const
{
    return (Matrix3(worldMatrix) * -Vector3::UnitZ()).Normalized();
}
