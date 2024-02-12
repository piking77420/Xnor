#pragma once

#include <cstdint>
#include <string>

#include "core.hpp"

class XNOR_ENGINE Guid
{
public:
    Guid();

    [[nodiscard]]
    bool operator==(const Guid& other) const;

    explicit operator std::string() const;

private:
    static constexpr inline size_t Data4Size = 8;
    
    uint32_t m_Data1;
    uint16_t m_Data2;
    uint16_t m_Data3;
    uint8_t  m_Data4[Data4Size];
};
