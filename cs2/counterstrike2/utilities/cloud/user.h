#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <mutex>
#include "../nlohmann/json.hpp"
#include "../xorstr.h"

struct ConfigSettingsT;
class c_flatbuffers;

enum EProcessingType {
	Auth,
	Avatar,
	GetConfigs,
	Load,
	Save,
	Rename,
	Delete,
	Create,
	Parse,
	GetOnline,
	DEV
};

struct ProcessingRecord {
	std::string share;
	std::string name;
	nlohmann::json json;
	bool changed;

	ProcessingRecord() {}
	ProcessingRecord(std::string index_, bool changed) : share(index_), changed(changed) {}
	ProcessingRecord(std::string index_, std::string name_, bool changed) : share(index_), name(name_), changed(changed) {}
	ProcessingRecord(nlohmann::json js) : json(js), changed(true), share() {}

	void ResetRecord() {
		share.clear();
		changed = false;
		name.clear();
		json.clear();
	}
};

class CConfig
{
public:
	std::string name;
	std::string owner;
	std::string user;
	std::string share;
	bool available_share;
	std::string created;
	std::string updated;
	std::vector<std::uint8_t> data_; // is used for GetMutableRoot / GetRoot with class Config
	bool is_deleted;
};

class Online_ {
public:
	std::string username;
	std::vector<std::uint8_t> avatar;
};

class CUser {
public:
	void Processing(const EProcessingType type, const ProcessingRecord processing_record = ProcessingRecord());
	std::unordered_map<std::string, CConfig>& GetConfigs() {
		return configs_;
	}

	void AddConfig(const std::string& uuid, const CConfig& config) {
		configs_[uuid] = config;
	}

	std::vector<Online_>& GetOnline() {
		return onlines_;
	}

	int GetAvatar(std::string username) {
		if (textures_.find(username) == textures_.end()) {
			textures_[username] = -1;
		}

		return textures_[username];
	}

	std::string GetUsername() {
		return username_;
	}

	void SetUsername(const std::string username) {
		username_ = username;
	}

	std::unordered_map<std::string, double>& GetConfig() {
		return config_map_;
	}

	double IsActive(std::string var, std::string bind_var = "", int index = -1);
private:
	void save(ProcessingRecord processing_record_);
	void rename(const ProcessingRecord& processing_record);
	void create();
	void Parse(ProcessingRecord processing_record_);
protected:
	std::uint64_t Hash(std::string str) {
		return std::hash<std::string>()(str);
	}
public:
	std::string username_ = "User";
	std::string prefix_;
	std::string id_;
	std::uint64_t subtime_;
	std::unordered_map<std::string, CConfig> configs_;
	std::vector<Online_> onlines_;
	std::vector < std::uint8_t> avatar;
	std::unordered_map<std::string, int> textures_;
	std::unordered_map<std::string, double> config_map_;
	std::shared_ptr<ConfigSettingsT> config_;
	std::mutex mutex_;
	bool m_should_processing;
	void LoadConfigsFromFiles(const std::string& directoryPath, std::unordered_map<std::string, CConfig>& configs);
	void delete_cfg(const ProcessingRecord& processing_record);
	void load(ProcessingRecord processing_record_);
};

inline auto g_user = std::make_unique<CUser>();