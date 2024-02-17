#include "entity_test.hpp"

void EntityTest::Begin() const
{
    CreateTypeInfo();
    Entity::Begin();
}

void EntityDamage::Begin() const
{
    CreateTypeInfo();
    Entity::Begin();
}
