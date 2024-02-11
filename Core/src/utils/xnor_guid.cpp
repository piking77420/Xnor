#include "utils/xnor_guid.hpp"

#include <Windows.h>
#include<Rpc.h>

#include "utils/logger.hpp"

#define DATA4_SIZE 8

bool XnorGuid::operator==(const XnorGuid& xnorGuid) const
{
	if (xnorGuid.Data1 != Data1)
		return false;

	if (xnorGuid.Data2 != Data2)
		return false;

	if (xnorGuid.Data3 != Data3)
		return false;

	for (size_t i = 0; i < DATA4_SIZE; i++)
	{
		if (xnorGuid.Data4[i] != Data4[i])
			return false;
	}

	return true;
}


XnorGuid::XnorGuid() : Data1(0),Data2(0),Data3(0)
{
	for (size_t i = 0; i < DATA4_SIZE; i++)
	{
		Data4[i] = 0;
	}

	HRESULT result = S_OK;
	result = CoCreateGuid(reinterpret_cast<UUID*>(this));

	if (result != S_OK)
	{
		Logger::LogError("shouldn't print this");
	}
}
