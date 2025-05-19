#pragma once

#include <iostream>
// used: CUtlBuffer
#include "../sdk/classes/c_utl_buffer.h"
#include "sdk.hpp"

namespace crc
{
	struct CInButtonStateNoVTable
	{
	public:
		std::uint64_t hold;
		std::uint64_t toggle;
		std::uint64_t scroll;
	};

	struct SavedData_t
	{
		CInButtonStateNoVTable nButtons;
		vector angView;
	};

	inline SavedData_t savedData;

	inline void save(c_user_cmd_base* pBaseCmd)
	{
		if (pBaseCmd->view != nullptr)
			savedData.angView = pBaseCmd->view->angles;

		savedData.nButtons.hold = pBaseCmd->buttons->hold;
		savedData.nButtons.toggle = pBaseCmd->buttons->toggle;
		savedData.nButtons.scroll = pBaseCmd->buttons->scroll;
	}

	inline void apply(c_user_cmd* pCmd)
	{
		c_user_cmd_base* pBaseCmd = pCmd->csgo_user_cmd.base;
		if (pBaseCmd == nullptr)
			return;

		pCmd->buttons.hold = savedData.nButtons.hold;
		pCmd->buttons.toggle = savedData.nButtons.toggle;
		pCmd->buttons.scroll = savedData.nButtons.scroll;

		if (pBaseCmd->view != nullptr)
			pBaseCmd->view->angles = savedData.angView;
	}

	inline bool calculate_crc(c_user_cmd_base* pBaseCmd)
	{
		int nCalcualtedCRCSize = pBaseCmd->get_cache_size();
		c_utl_buffer protobufBuffer/*(0, 0, 0)*/;
		protobufBuffer.ensure(nCalcualtedCRCSize + 1);

		using fnSerializePartialToArray = bool(__fastcall*)(c_user_cmd_base*, c_utl_buffer, int);
		static const fnSerializePartialToArray oSerializePartialToArray = reinterpret_cast<fnSerializePartialToArray>(g_utils->pattern_scan(g_cs2->m_module_system.get_client(), "48 89 5C 24 18 55 56 57 48 81 EC 90").get());

		if (oSerializePartialToArray(pBaseCmd, protobufBuffer, nCalcualtedCRCSize))
		{
			std::uintptr_t* pMessage = reinterpret_cast<uintptr_t*>(Interfaces::m_mem_alloc->Alloc(0x18));
			pBaseCmd->m_cached_bits |= 1;
			auto nHasBits = static_cast<uint32_t>(pBaseCmd->m_has_bits & 0xFFFFFFFFFFFFFFFC);
			if ((pBaseCmd->m_has_bits & 1) != 0)
				nHasBits = static_cast<uint32_t>(nHasBits);

			using fnWriteMessage = void(__fastcall*)(std::uintptr_t*, c_utl_buffer, int);
			static const fnWriteMessage oWriteMessage = reinterpret_cast<fnWriteMessage>(g_utils->pattern_scan(g_cs2->m_module_system.get_client(), "48 89 5C 24 10 48 89 6C 24 18 48 89 7C 24 20 41 56 48 83 EC 20 48 BF").get());

			using fnSetMessageData = std::string*(__fastcall*)(void*, std::uintptr_t*, void*);
			static const fnSetMessageData oSetMessageData = reinterpret_cast<fnSetMessageData>(g_utils->pattern_scan(g_cs2->m_module_system.get_client(), "48 89 5C 24 20 55 56 57 48 83 EC 30 49").get());

			oWriteMessage(pMessage, protobufBuffer, nCalcualtedCRCSize);
			pBaseCmd->move_crc = oSetMessageData(&pBaseCmd->move_crc, pMessage, &nHasBits);
			pBaseCmd->set_bits(EBaseCmdBits::BASE_BITS_MOVE_CRC);
			Interfaces::m_mem_alloc->Free(pMessage);

			return true;
		}

		return false;
	}
}