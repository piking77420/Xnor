#pragma once

#include <cstdint>
#include <string>

#include "core.hpp"

BEGIN_XNOR_CORE

class XNOR_ENGINE Guid final
{
public:
    constexpr Guid() = default;

    static Guid New();

    [[nodiscard]]
    bool operator==(const Guid& other) const;

    explicit operator std::string() const;

private:
    static constexpr inline uint64_t Data4Size = 8;
    
    uint32_t m_Data1 = 0;
    uint16_t m_Data2 = 0;
    uint16_t m_Data3 = 0;
    uint8_t  m_Data4[Data4Size] = {};
};

END_XNOR_CORE
