#pragma once

#include "core.hpp"

BEGIN_XNOR_CORE

class Seriable
{
public:
    Seriable() = default;
    
    virtual ~Seriable();

    DEFAULT_COPY_MOVE_OPERATIONS(Seriable)

    // TO DO SHOULBE BE PURE VIRTUAL
    virtual void OnSerialize() const;
    
    // TO DO SHOULBE BE PURE VIRTUAL
    virtual void OnDeSerialize() const;
    
private:
    
};

END_XNOR_CORE