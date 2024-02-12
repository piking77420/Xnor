#include "guid.hpp"

#include <Windows.h>
#include <Rpc.h>

#include "utils/logger.hpp"

Guid::Guid()
	: m_Data1(0)
	, m_Data2(0)
	, m_Data3(0)
{
	for (unsigned char& i : m_Data4)
		i = 0;

	const HRESULT result = CoCreateGuid(reinterpret_cast<UUID*>(this));

	if (result != S_OK)
		Logger::LogError("Couldn't create GUID");
}

bool Guid::operator==(const Guid& other) const
{
	if (other.m_Data1 != m_Data1)
		return false;

	if (other.m_Data2 != m_Data2)
		return false;

	if (other.m_Data3 != m_Data3)
		return false;

	for (size_t i = 0; i < Data4Size; i++)
	{
		if (other.m_Data4[i] != m_Data4[i])
			return false;
	}

	return true;
}
