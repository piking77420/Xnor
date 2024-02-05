#pragma once

class Scene;


class Component
{
public:

    virtual void Begin()
    {
    
    }

    virtual void Update()
    {
        
    }
    
    virtual  ~Component() = default;
};
