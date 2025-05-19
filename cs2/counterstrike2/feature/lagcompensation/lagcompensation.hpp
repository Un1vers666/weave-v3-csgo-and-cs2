#pragma once

#include <deque>

#include "../../sdk/sdk.hpp"

class c_bone
{
public:
    void copy();
    void set();
protected:
    c_bone_data matrix_[128];
};

class c_lagrecord
{
private:
    float m_simtime_;
    bool breaking_lag_comp{};
    int choked;
public:
    c_bone_data matrix_[128];
    int bone_count;
    vector origin;
    vector abs_origin;
    vector angle_rotation;
    vector abs_angle_rotation;
    vector eye_angles;
    vector velocity;
    vector abs_velocity;
    int flags;
private:
    c_bone_data backup_matrix_[128];
public:
    void setup(c_cs_player_pawn* player_pawn);
    void apply(c_cs_player_pawn* player_pawn, bool extrapolation = false, bool predicted = false);
    void restore(c_cs_player_pawn* player_pawn);
    bool record_of_bounds();
public:
    float get_sim_time() const { return m_simtime_; }
    bool valid{};
    bool last_valid{};

    bool is_valid(float min_delta) const {
        return  this->m_simtime_ > min_delta;
    }
};

class c_animation_table
{
public:
    std::deque<c_lagrecord> m_lag_records_{};
    c_lagrecord latest_record{};
    c_lagrecord oldest_record{};
};

// In lagcompensation.hpp
class c_lagcompensation
{
public:
    void run();
public:
    float get_delta_time(int tick_entity);
public:
    c_animation_table* get_animation(c_cs_player_pawn* player_pawn);
private:
    std::unordered_map<unsigned long, c_animation_table> m_animation{};
public:
    int level;
};

inline auto g_lagcompensation = std::make_unique<c_lagcompensation>();