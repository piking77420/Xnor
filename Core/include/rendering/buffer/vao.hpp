#pragma once
#include "core.hpp"

BEGIN_XNOR_CORE

class XNOR_ENGINE VAO
{
public:

    DEFAULT_COPY_MOVE_OPERATIONS(VAO)

    void Init();

    VAO() = default;

    ~VAO();

    void BindBuffer() const;

    void UnBindBuffer() const;

private:
    uint32_t m_Id = 0;
};

END_XNOR_CORE