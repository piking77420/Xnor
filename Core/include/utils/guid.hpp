#pragma once

#include "core.hpp"

class XNOR_ENGINE Guid
{
public:
    Guid();

    [[nodiscard]]
    bool operator==(const Guid& other) const;

private:
    static constexpr inline unsigned int Data4Size = 8;
    
    unsigned long  m_Data1;
    unsigned short m_Data2;
    unsigned short m_Data3;
    unsigned char  m_Data4[Data4Size];
};

