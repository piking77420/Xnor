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
    REFLECTABLE_IMPL(EntityDamage)

public:
    XNOR_ENGINE void Begin() const override;

private:
    uint32_t m_Damage;
};

REFL_AUTO(type(EntityDamage, bases<XnorCore::Entity>),
    field(m_Damage)
)
