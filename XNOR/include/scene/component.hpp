#pragma once
#include "scene.hpp"

class Scene;
class Entity;

class Component
{
public:

    Entity* entity;

    virtual ~Component() = default;

    virtual void Begin()
    {
       
    }

    virtual void Update()
    {
       
    }
};
