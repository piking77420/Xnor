#include "guid.hpp"

#include <Windows.h>
#include <Rpc.h>

#include "utils/logger.hpp"

XnorCore::Guid::Guid()
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

bool XnorCore::Guid::operator==(const Guid& other) const
{
	return (
	   reinterpret_cast<const uint32_t*>(this)[0] == reinterpret_cast<const uint32_t*>(&other)[0] &&
	   reinterpret_cast<const uint32_t*>(this)[1] == reinterpret_cast<const uint32_t*>(&other)[1] &&
	   reinterpret_cast<const uint32_t*>(this)[2] == reinterpret_cast<const uint32_t*>(&other)[2] &&
	   reinterpret_cast<const uint32_t*>(this)[3] == reinterpret_cast<const uint32_t*>(&other)[3]
	);
}

XnorCore::Guid::operator std::string() const
{
	return std::format("{:X}-{:X}-{:X}-{:X}{:X}{:X}{:X}{:X}{:X}{:X}{:X}", m_Data1, m_Data2, m_Data3, m_Data4[0], m_Data4[1], m_Data4[2], m_Data4[3], m_Data4[4], m_Data4[5], m_Data4[6], m_Data4[7]);
}
