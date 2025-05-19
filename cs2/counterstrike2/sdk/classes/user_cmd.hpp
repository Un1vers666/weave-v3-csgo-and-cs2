#pragma once

#include <cstdint>
#include "../math/vector.hpp"
#include "../proto/proto.h"

enum EButtonStatePBBits : uint32_t
{
    BUTTON_STATE_PB_BITS_BUTTONSTATE1 = 0x1U,
    BUTTON_STATE_PB_BITS_BUTTONSTATE2 = 0x2U,
    BUTTON_STATE_PB_BITS_BUTTONSTATE3 = 0x4U
};

enum ESubtickMoveStepBits : std::uint32_t
{
    MOVESTEP_BITS_BUTTON = 0x1U,
    MOVESTEP_BITS_PRESSED = 0x2U,
    MOVESTEP_BITS_WHEN = 0x4U,
    MOVESTEP_BITS_ANALOG_FORWARD_DELTA = 0x8U,
    MOVESTEP_BITS_ANALOG_LEFT_DELTA = 0x10U
};

enum EInputHistoryBits : std::uint32_t
{
    INPUT_HISTORY_BITS_VIEWANGLES = 0x1U,
    INPUT_HISTORY_BITS_SHOOTPOSITION = 0x2U,
    INPUT_HISTORY_BITS_TARGETHEADPOSITIONCHECK = 0x4U,
    INPUT_HISTORY_BITS_TARGETABSPOSITIONCHECK = 0x8U,
    INPUT_HISTORY_BITS_TARGETANGCHECK = 0x10U,
    INPUT_HISTORY_BITS_CL_INTERP = 0x20U,
    INPUT_HISTORY_BITS_SV_INTERP0 = 0x40U,
    INPUT_HISTORY_BITS_SV_INTERP1 = 0x80U,
    INPUT_HISTORY_BITS_PLAYER_INTERP = 0x100U,
    INPUT_HISTORY_BITS_RENDERTICKCOUNT = 0x200U,
    INPUT_HISTORY_BITS_RENDERTICKFRACTION = 0x400U,
    INPUT_HISTORY_BITS_PLAYERTICKCOUNT = 0x800U,
    INPUT_HISTORY_BITS_PLAYERTICKFRACTION = 0x1000U,
    INPUT_HISTORY_BITS_FRAMENUMBER = 0x2000U,
    INPUT_HISTORY_BITS_TARGETENTINDEX = 0x4000U
};

enum EBaseCmdBits : std::uint32_t
{
    BASE_BITS_MOVE_CRC = 0x1U,
    BASE_BITS_BUTTONPB = 0x2U,
    BASE_BITS_VIEWANGLES = 0x4U,
    BASE_BITS_COMMAND_NUMBER = 0x8U,
    BASE_BITS_CLIENT_TICK = 0x10U,
    BASE_BITS_FORWARDMOVE = 0x20U,
    BASE_BITS_LEFTMOVE = 0x40U,
    BASE_BITS_UPMOVE = 0x80U,
    BASE_BITS_IMPULSE = 0x100U,
    BASE_BITS_WEAPON_SELECT = 0x200U,
    BASE_BITS_RANDOM_SEED = 0x400U,
    BASE_BITS_MOUSEDX = 0x800U,
    BASE_BITS_MOUSEDY = 0x1000U,
    BASE_BITS_CONSUMED_SERVER_ANGLE = 0x2000U,
    BASE_BITS_CMD_FLAGS = 0x4000U,
    BASE_BITS_ENTITY_HANDLE = 0x8000U
};

enum ECSGOUserCmdBits : std::uint32_t
{
    CSGOUSERCMD_BITS_BASECMD = 0x1U,
    CSGOUSERCMD_BITS_LEFTHAND = 0x2U,
    CSGOUSERCMD_BITS_ATTACK3START = 0x4U,
    CSGOUSERCMD_BITS_ATTACK1START = 0x8U,
    CSGOUSERCMD_BITS_ATTACK2START = 0x10U
};

enum InputBitMask_t
{
    IN_NONE = 0x0,
    IN_ALL = 0xffffffffffffffff,
    IN_ATTACK = 0x1,
    IN_JUMP = 0x2,
    IN_DUCK = 0x4,
    IN_FORWARD = 0x8,
    IN_BACK = 0x10,
    IN_USE = 0x20,
    IN_TURNLEFT = 0x80,
    IN_TURNRIGHT = 0x100,
    IN_MOVELEFT = 0x200,
    IN_MOVERIGHT = 0x400,
    IN_ATTACK2 = 0x800,
    IN_RELOAD = 0x2000,
    IN_SPEED = 0x10000,
    IN_JOYAUTOSPRINT = 0x20000,
    IN_FIRST_MOD_SPECIFIC_BIT = 0x100000000,
    IN_USEORRELOAD = 0x100000000,
    IN_SCORE = 0x200000000,
    IN_ZOOM = 0x400000000,
    IN_LOOK_AT_WEAPON = 0x800000000
};

enum EInButtonState : uint8_t
{
    IN_BUTTON_UP = 0,
    IN_BUTTON_DOWN,
    IN_BUTTON_DOWN_UP,
    IN_BUTTON_UP_DOWN,
    IN_BUTTON_UP_DOWN_UP,
    IN_BUTTON_DOWN_UP_DOWN,
    IN_BUTTON_DOWN_UP_DOWN_UP,
    IN_BUTTON_UP_DOWN_UP_DOWN,
    IN_BUTTON_STATE_COUNT
};

enum EButtonType : uint8_t
{
    HOLD,
    PRESSED,
    UNPRESSED
};

template <typename T>
struct RepeatedPtrField_t
{
    struct Rep_t
    {
        int m_nAllocatedSize;
        T* m_tElements[(std::numeric_limits<int>::max() - 2 * sizeof(int)) / sizeof(void*)];
    };

    void* m_pArena;
    int m_nCurrentSize;
    int m_nTotalSize;
    Rep_t* m_pRep;
};

class c_base_pb
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
};

static_assert(sizeof(c_base_pb) == 0x18);

class c_cmd_qangle : public c_base_pb {
public:
    // char pad_001[0x18];
    vector angles;
};

static_assert(sizeof(c_cmd_qangle) == 0x28);

class c_cmd_buttons : public c_base_pb
{
public:
    std::uint64_t hold;
    std::uint64_t toggle;
    std::uint64_t scroll;
};
static_assert(sizeof(c_cmd_buttons) == 0x30);

class csgo_interpolation_info_pb : public c_base_pb
{
public:
    // char pad[0x18];
    float fraction;
    int src_tick;
    int dst_tick;
};

static_assert(sizeof(csgo_interpolation_info_pb) == 0x28);

class csgo_input_history_entry : public c_base_pb
{
public:
    c_cmd_qangle* angle; // 0x18
    c_cmd_qangle* pShootPosition; // 0x20
    c_cmd_qangle* pTargetHeadPositionCheck; // 0x28
    c_cmd_qangle* pTargetAbsPositionCheck; // 0x30
    c_cmd_qangle* pTargetAngPositionCheck; // 0x38
    csgo_interpolation_info_pb* cl_interp; // 0x40
    csgo_interpolation_info_pb* sv_interp0; // 0x48
    csgo_interpolation_info_pb* sv_interp1; // 0x50 
    csgo_interpolation_info_pb* player_interp; // 0x58
    int render_tick_count; // 0x60
    float render_fraction; // 0x64
    int player_tick_count; // 0x68
    float player_fraction; // 0x6C
    int frame_number; // 0x70
    int nTargetEntIndex; // 0x74

    csgo_interpolation_info_pb* create_new_interp();
    c_cmd_qangle* create_msg_angle();
};

static_assert(sizeof(csgo_input_history_entry) == 0x78);

class c_subtick_move_step : public c_base_pb
{
public:
    std::uint64_t buttons;
    bool pressed;
    float when;
    float fl_analog_forward_delta;
    float fl_analog_left_delta;
    std::int16_t unk;
    std::int8_t unk2;
};

class c_user_cmd_base : public c_base_pb
{
public:
    RepeatedPtrField_t<c_subtick_move_step> subtick_move;
    std::string* move_crc;
    c_cmd_buttons* buttons;
    c_cmd_qangle* view;
    std::int32_t sequence;
    std::int32_t tick_count;
    float forwardmove;
    float sidemove;
    float upmove;
    std::uint32_t impulse;
    std::uint32_t weapon_select;
    std::uint32_t random_seed;
    std::uint32_t mousedx;
    std::uint32_t mousedy;
    std::uint32_t consumed_server_angles_changes;
    std::int32_t cmd_flags;
    std::uint32_t pawn_entity_handle;

    void Set(int button, bool pressed);
    void UnSet(int button, bool pressed);
    int get_cache_size();
};

static_assert(sizeof(c_user_cmd_base) == 0x80);

class ccsgo_user_cmd_pb
{
public:
    std::uint32_t nHasBits;
    std::uint64_t nCachedSize;
    RepeatedPtrField_t<csgo_input_history_entry> input_history;
    c_user_cmd_base* base;
    bool bLeftHandDesired;
    std::uint32_t attack_history_index[3];

    void set_bits(std::uint32_t nBits)
    {
            nHasBits |= nBits;
    }
};

static_assert(sizeof(ccsgo_user_cmd_pb) == 0x40);

struct button_cmd
{
    char pad000[0x8];
    uint64_t hold;
    uint64_t toggle;
    uint64_t scroll;
};

static_assert(sizeof(button_cmd) == 0x20);

class c_user_cmd {
public:

    char pad000[0x8];
    char pad001[0x10];
    ccsgo_user_cmd_pb csgo_user_cmd; // 0x8
     button_cmd buttons; // 0x28
     char pad0002[0x10];
     bool has_been_prediction;
     char pad0003[0x9];
     char pad0004[0x6];
public:
    void set_input_history_angle(vector& angles);
    void override_angle(const vector& angView);
    c_subtick_move_step* create_subtick_step(void* arena);
    c_subtick_move_step* create_subtick();
    void add_subtick(bool pressed, int button, bool needs_delete = false);
    void set_attack_history(int index);
    void set_button_state(std::uint64_t button, EInButtonState call_type);
    void remove_button_state(std::uint64_t button);
    
    void override_tick(int history_size, int tick_count, float fraction)
    {
        for (std::int32_t i = 0; i < history_size; i++)
        {
            csgo_input_history_entry* tick = csgo_user_cmd.input_history.m_pRep->m_tElements[csgo_user_cmd.attack_history_index[0]];
    
            if (tick->cl_interp == nullptr)
                tick->cl_interp = tick->create_new_interp();
    
            if (tick->sv_interp0 == nullptr)
                tick->sv_interp0 = tick->create_new_interp();
    
            if (tick->sv_interp1 == nullptr)
                tick->sv_interp1 = tick->create_new_interp();
   
    
            tick->m_cached_bits |= 0x800u;
            tick->render_tick_count = tick_count + 1;
          //  tick->m_cached_bits |= 0x1000u;
           // tick->render_fraction = fraction;
           
            if (tick->cl_interp && tick->sv_interp0 && tick->sv_interp1)
            {
                tick->cl_interp->m_cached_bits |= 2u;
                auto cl_cachedbits = tick->cl_interp->m_cached_bits;
                tick->cl_interp->src_tick = tick_count;
                cl_cachedbits |= 4;
                tick->cl_interp->m_cached_bits = cl_cachedbits;
                tick->cl_interp->dst_tick = tick_count + 1;
                tick->cl_interp->m_cached_bits = cl_cachedbits | 1;
               // tick->cl_interp->fraction = fraction;
                tick->cl_interp->m_cached_bits |= 0x40u;
    
    
                tick->sv_interp0->m_cached_bits |= 2u;
                auto sv_cachedbits_0 = tick->sv_interp0->m_cached_bits;
                tick->sv_interp0->src_tick = tick_count;
                sv_cachedbits_0 |= 4;
                tick->sv_interp0->m_cached_bits = sv_cachedbits_0;
                tick->sv_interp0->dst_tick = tick_count + 1;
                tick->sv_interp0->m_cached_bits = sv_cachedbits_0 | 1;
               // tick->sv_interp0->fraction = 0.f;
                tick->sv_interp0->m_cached_bits |= 0x80u;
    
    
                tick->sv_interp1->m_cached_bits |= 2u;
                auto sv_cachedbits_1 = tick->sv_interp1->m_cached_bits;
                tick->sv_interp1->src_tick = tick_count + 1;
                sv_cachedbits_1 |= 4;
                tick->sv_interp1->m_cached_bits = sv_cachedbits_1;
                tick->sv_interp1->dst_tick = tick_count + 2;
                tick->sv_interp1->m_cached_bits = sv_cachedbits_1 | 1;
               // tick->sv_interp1->fraction = 0.f;
            }
        }
    }
};

static_assert(sizeof(c_user_cmd) == 0x98);