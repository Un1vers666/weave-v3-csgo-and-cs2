#pragma once
#include <cstdint>

struct grenade_positions
{
	int size;
	char pad[0x4];
	vector* data;
	std::uint64_t unk;
};

class c_grenade_trace
{
public:
	char pad[0xD08];
	std::uint64_t size;
	grenade_positions* data;
public:
	float duration() {
		return *(float*)(std::uintptr_t(this) + 0xCE8);
	}
};