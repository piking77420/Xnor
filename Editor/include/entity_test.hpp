#pragma once

#include "scene/entity.hpp"

class EntityTest : public XnorCore::Entity
{
    REFLECTABLE_IMPL(EntityTest)

public:
    XNOR_ENGINE void Begin() const override;

private:
    uint32_t m_Health;
};

REFL_AUTO(type(EntityTest, bases<XnorCore::Entity>),
    field(m_Health)
)

class EntityDamage : public XnorCore::Entity
{
public:
    EntityDamage();
    
    REFLECTABLE_IMPL(EntityDamage)

public:
    XNOR_ENGINE void Begin() const override;

public:
    int32_t m_Int32;
    float_t m_Float;
    Vector2 m_Vec2;
    uint8_t m_Uint8;
    Vector3 m_Vec3;
    int32_t m_ArrayInt[4];
    Vector2 m_ArrayVec2[4];
    XnorCore::Transform m_ArrayTransform[2];
};

REFL_AUTO(type(EntityDamage, bases<XnorCore::Entity>),
    field(m_Int32),
    field(m_Float),
    field(m_Vec2),
    field(m_Uint8),
    field(m_Vec3),
    field(m_ArrayInt),
    field(m_ArrayVec2),
    field(m_ArrayTransform)
)
