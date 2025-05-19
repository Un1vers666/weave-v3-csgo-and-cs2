#pragma once
#include <cstdint>

#define INVALID_EHANDLE_INDEX 0xFFFFFFFF
#define ENT_ENTRY_MASK 0x7FFF
#define NUM_SERIAL_NUM_SHIFT_BITS 15
// @source: https://developer.valvesoftware.com/wiki/Entity_limit#Source_2_limits
#define ENT_MAX_NETWORKED_ENTRY 16384

#define CS_DEBUG_BREAK() __debugbreak()
#define CS_ASSERT(EXPRESSION) static_cast<void>(!!(EXPRESSION) || (CS_DEBUG_BREAK(), 0))


class c_base_handle
{
public:
	c_base_handle() noexcept :
		nIndex(INVALID_EHANDLE_INDEX) { }

	c_base_handle(const int nEntry, const int nSerial) noexcept
	{
		CS_ASSERT(nEntry >= 0 && (nEntry & ENT_ENTRY_MASK) == nEntry);
		CS_ASSERT(nSerial >= 0 && nSerial < (1 << NUM_SERIAL_NUM_SHIFT_BITS));

		nIndex = nEntry | (nSerial << NUM_SERIAL_NUM_SHIFT_BITS);
	}

	bool operator!=(const c_base_handle& other) const noexcept
	{
		return nIndex != other.nIndex;
	}

	bool operator==(const c_base_handle& other) const noexcept
	{
		return nIndex == other.nIndex;
	}

	bool operator<(const c_base_handle& other) const noexcept
	{
		return nIndex < other.nIndex;
	}

	[[nodiscard]] bool IsValid() const noexcept
	{
		return nIndex != INVALID_EHANDLE_INDEX;
	}

	[[nodiscard]] int GetEntryIndex() const noexcept
	{
		return static_cast<int>(nIndex & ENT_ENTRY_MASK);
	}

	[[nodiscard]] int GetSerialNumber() const noexcept
	{
		return static_cast<int>(nIndex >> NUM_SERIAL_NUM_SHIFT_BITS);
	}

private:
	std::uint32_t nIndex;

};
