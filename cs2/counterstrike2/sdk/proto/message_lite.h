#pragma once

class message_lite
{
public:
	char pad1[0x8]; // 0x0 VTABLE
	std::uint32_t m_has_bits; // 0x8
	std::uint64_t m_cached_bits; // 0xC

	// @note: this function is used to check if the bits are set and set them if they are not
	void set_bits(std::uint64_t nBits)
	{
		m_cached_bits |= nBits;
	}

	bool serialize_partial_to_array(std::uintptr_t* data, int size);
	void parse_partial_to_array(const void* data, int size);
	std::uintptr_t* merge(void* to, std::uintptr_t* hyeta);
public:
	internal_metadata _internal_metadata_;
};