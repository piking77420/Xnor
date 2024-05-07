#pragma once
#include "core.hpp"

BEGIN_XNOR_CORE

class XNOR_ENGINE VAO
{
public:

    DEFAULT_COPY_MOVE_OPERATIONS(VAO)

    VAO();

    ~VAO();

    void BindBuffer() const;

    void UnBindBuffer() const;

private:
    uint32_t m_Id;
};

END_XNOR_CORE