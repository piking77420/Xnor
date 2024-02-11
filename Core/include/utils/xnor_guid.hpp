#pragma once

#include "core.hpp"

class XNOR_ENGINE XnorGuid
{
public:

    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];

    bool operator==(const XnorGuid& xnorGuid) const;

    XnorGuid();
};

