#include "physics/components/box_collider.hpp"

#include "physics/physics_world.hpp"
#include "physics/components/collider.hpp"
#include "scene/entity.hpp"
#include "serialization/serializer.hpp"

using namespace XnorCore;

REFLECTABLE_IMPL_CPP(BoxCollider)

void BoxCollider::Begin()
{
    const Transform& t = entity->transform;
    /*const Matrix trs = Matrix::Trs(m_Center, m_Rotation, m_Size);
    Matrix result = t.worldMatrix * trs;

    Vector3 position;
    Quaternion q;
    Vector3 s;
    Vector3 skew;
    Vector4 p;
    result.Decompose(&position, &q, &s, &skew, &p);
    m_BodyId = PhysicsWorld::CreateBox(position, q, s);*/

    m_BodyId = PhysicsWorld::CreateBox(this, t.GetPosition(), t.GetRotation(), t.GetScale(), m_IsTrigger);
}

void BoxCollider::Update()
{
    Collider::Update();
}
