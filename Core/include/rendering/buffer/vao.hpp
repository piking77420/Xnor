#pragma once
#include "core.hpp"
#include "rendering/rhi_typedef.hpp"

BEGIN_XNOR_CORE
class XNOR_ENGINE  VAO
{
public:

    DEFAULT_COPY_MOVE_OPERATIONS(VAO)

    void Init();

    VAO() = default;

    ~VAO();

    void BindBuffer() const;

    void UnBindBuffer() const;

    uint32_t GetId() const;

    void ComputeDescriptor(const VaoDescriptor& vaoDescriptor) const;

private:
    uint32_t m_Id = 0;
};

END_XNOR_CORE