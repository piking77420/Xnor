#include "entity_test.hpp"

void EntityTest::Begin() const
{
    CreateTypeInfo();
    Entity::Begin();
}

EntityDamage::EntityDamage()
{
    m_ArrayTransform[0].scale.y = 52;
}

void EntityDamage::Begin() const
{
    CreateTypeInfo();
    Entity::Begin();
}
