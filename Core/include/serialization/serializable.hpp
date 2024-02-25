#pragma once

#include "core.hpp"

BEGIN_XNOR_CORE

class XNOR_ENGINE Serializable
{
public:
    Serializable() = default;
    
    virtual ~Serializable() = default;

    DEFAULT_COPY_MOVE_OPERATIONS_NO_ENGINE(Serializable)
    
    virtual void Serialize() const;
    
    virtual void Deserialize();
private:
    
};

END_XNOR_CORE