#pragma once

#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include <deque>
#include <mutex>

enum e_hotkey_mode
{
	hold = 0,
	toggle
};

struct HotkeyUnique
{
	int key;
	int mode;
	bool visible;
	int type;
	std::vector<double> value;

	// Define less-than operator for sorting in set
	bool operator<(const HotkeyUnique& other) const
	{
		return std::tie(key, mode, visible, type, value) < std::tie(other.key, other.mode, other.visible, other.type, other.value);
	}
};

class hotkey_impl
{
public:
	int key;
	int mode;
	bool visible;
	int type;
	std::vector<double> value;
	int current;
	bool enabled;
};

class hotkeys {
public:
	int current;
	int store_current;
	std::string name_;
	std::vector<std::string> name_visuals;
	std::vector<std::string> name;
	std::deque< hotkey_impl> binds;
public:
	std::vector<std::string> get_binds();
};

class hotkey
{
public:
	enum e_hotkey_list
	{
		thirdperson,
		autopeek,

		hotkey_max
	};

	enum e_hotkey_mode
	{
		hold = 0,
		toggle
	};

public:
	hotkey() = default;
	hotkey(int key, int mode, std::string name) : key{ key }, mode{ mode }, name{ name } {};
public:
	void set_key(int new_key) { key = new_key; };
	int get_key() { return key; };
public:
	void set_mode(int new_mode) { mode = new_mode; };
	int get_mode() { return mode; };
public:
	int key = -1;
	int mode;
	std::string name;
};

class c_hotkey
{
public:
	void update();
public:
	std::unique_ptr<std::unordered_map<std::uint64_t, hotkeys>> hotkeys_ = std::make_unique<std::unordered_map<std::uint64_t, hotkeys>>();
	std::mutex mutex_;
};

inline auto g_hotkey = std::make_unique<c_hotkey>();