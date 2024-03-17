#pragma once

#include <Coral/Type.hpp>

#include "core.hpp"

BEGIN_XNOR_CORE

class DotnetType
{
public:
    Coral::Type* GetCoralType();
    
    const Coral::Type* GetCoralType() const;

private:
    Coral::Type* m_Type = nullptr;
};

END_XNOR_CORE
