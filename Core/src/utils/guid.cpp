#include "utils/guid.hpp"

#include <Windows.h>

#include "utils/logger.hpp"

using namespace XnorCore;

Guid Guid::New()
{
	Guid guid;

	const HRESULT result = CoCreateGuid(reinterpret_cast<UUID*>(&guid));

	if (result != S_OK)
		Logger::LogError("Couldn't create GUID");

	return guid;
}

bool_t Guid::operator==(const Guid& other) const
{
	return reinterpret_cast<const uint64_t*>(this)[0] == reinterpret_cast<const uint64_t*>(&other)[0] &&
		   reinterpret_cast<const uint64_t*>(this)[1] == reinterpret_cast<const uint64_t*>(&other)[1];
}

void Guid::FromString(const char_t* str)
{
	(void)sscanf_s(str, "%x-%hx-%hx-%hhx-%hhx-%hhx-%hhx-%hhx-%hhx-%hhx-%hhx", &m_Data1, &m_Data2, &m_Data3, &m_Data4[0], &m_Data4[1], &m_Data4[2], &m_Data4[3], &m_Data4[4], &m_Data4[5], &m_Data4[6], &m_Data4[7]);
}

Guid::operator std::string() const
{
	return std::format("{:X}-{:X}-{:X}-{:X}-{:X}-{:X}-{:X}-{:X}-{:X}-{:X}-{:X}", m_Data1, m_Data2, m_Data3, m_Data4[0], m_Data4[1], m_Data4[2], m_Data4[3], m_Data4[4], m_Data4[5], m_Data4[6], m_Data4[7]);
}
