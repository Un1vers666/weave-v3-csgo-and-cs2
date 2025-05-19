#pragma once

#ifndef SCHEMAOFFSET
#define SCHEMAOFFSET

#include "sdk.hpp"

class  xored_schema
{
public:
//	struct alignas(16) utl_xor_hash
//	{
//		std::uint32_t key;
//		char pad[0x8];
//		std::uint32_t hash_key;
//		char pad2[0x15];
//		std::uint32_t xored;
//		void* naebka;
//		std::uint32_t xored_hash;
//	};
//	 std::uint32_t() = default;
//	 std::uint32_t(const std::uint32_t offset)
//	{
//		map.key = get_hash<0x8936721837>(0x673521098);
//		map.hash_key = get_hash<0x89732193213>(map.key);
//		map.xored = offset ^ map.key;
//		map.xored_hash = map.xored ^ get_hash(get_hash<0x4562321983721>(map.hash_key));
//	}
//
//	 std::uint32_t(const std::uint32_t offset_, const std::uint32_t key_, const std::uint32_t hash_key_)
//	{
//		key = key_;
//		hash_key = hash_key_;
//		xored_key = offset_;
//	}
//
//	std::uint32_t get() {
//		return xored_key ^ key ^ get_hash(hash_key ^ 0x4562321983721);
//	}
//
//	__forceinline operator size_t() {
//		return get();
//	}
//private:
//	template<std::uint32_t key = 0x9863217634217321>
//	std::size_t constexpr get_hash(const std::size_t& value)
//	{
//		return std::integral_constant<uint64_t, key>::value^ value;
//	}
//public:
//	utl_xor_hash map;
//	std::uint32_t key;
//	std::uint32_t hash_key;
//	std::uint32_t xored_key;
};

class CSchemaOffset {
public:
	void init();
public:
	 std::uint32_t m_flNextAttack;
	 std::uint32_t m_hMyWeapons;
	 std::uint32_t m_hActiveWeapon;
	 std::uint32_t m_hViewModel;
	 std::uint32_t m_hPlayerPing;
	 std::uint32_t m_bForcedObserverMode;
	 std::uint32_t m_iObserverLastMode;
	 std::uint32_t m_hObserverTarget;
	 std::uint32_t m_iObserverMode;
	 std::uint32_t m_flSurfaceFriction;
	 std::uint32_t m_flDuckAmount;
	 std::uint32_t m_bOldJumpPressed;
	 std::uint32_t m_flLeftMove;
	 std::uint32_t m_flForwardMove;
	 std::uint32_t m_flMaxspeed;
	 std::uint32_t m_bHasHeavyArmor;
	 std::uint32_t m_bHasHelmet;
	 std::uint32_t m_bHasDefuser;
	 std::uint32_t m_nPersonaDataPublicLevel;
	 std::uint32_t m_PostProcessingVolumes;
	 std::uint32_t m_hViewEntity;
	 std::uint32_t m_flDeathCamTilt;
	 std::uint32_t m_flLastShotFOV;
	 std::uint32_t m_fOverrideFogEnd;
	 std::uint32_t m_fOverrideFogStart;
	 std::uint32_t m_bOverrideFogStartEnd;
	 std::uint32_t m_OverrideFogColor;
	 std::uint32_t m_bOverrideFogColor;
	 std::uint32_t m_hOldFogController;
	 std::uint32_t m_CurrentFog;
	 std::uint32_t m_hTonemapController;
	 std::uint32_t m_hZoomOwner;
	 std::uint32_t m_iFOVStart;
	 std::uint32_t m_iFOV;
	 std::uint32_t m_hCtrl;
	 std::uint32_t m_totalHitsOnServer;
	 std::uint32_t m_bIsRescuing;
	 std::uint32_t m_nNumBullets;
	 std::uint32_t m_iMaxClip1;
	 std::uint32_t m_flRangeModifier;
	 std::uint32_t m_flRange;
	 std::uint32_t m_flPenetration;
	 std::uint32_t m_flArmorRatio;
	 std::uint32_t m_flHeadshotMultiplier;
	 std::uint32_t m_nDamage;
	 std::uint32_t m_nSpreadSeed;
	 std::uint32_t m_nRecoilSeed;
	 std::uint32_t m_flSpread;
	 std::uint32_t m_flMaxSpeed;
	 std::uint32_t m_bHasBurstMode;
	 std::uint32_t m_szName;
	 std::uint32_t m_bIsRevolver;
	 std::uint32_t m_WeaponType;
	 std::uint32_t m_flPostponeFireReadyFrac;
	 std::uint32_t m_bInReload;
	 std::uint32_t m_nPostponeFireReadyTicks;
	 std::uint32_t m_iOriginalTeamNumber;
	 std::uint32_t m_nFallbackPaintKit;
	 std::uint32_t m_OriginalOwnerXuidHigh;
	 std::uint32_t m_OriginalOwnerXuidLow;
	 std::uint32_t m_seqFireSecondary;
	 std::uint32_t m_seqFirePrimary;
	 std::uint32_t m_seqIdle;
	 std::uint32_t m_iRecoilIndex;
	 std::uint32_t m_modelState;
	 std::uint32_t m_hPlayer;
	 std::uint32_t m_hBombDefuser;
	 std::uint32_t m_bBombDefused;
	 std::uint32_t m_flDefuseCountDown;
	 std::uint32_t m_flDefuseLength;
	 std::uint32_t m_flTimerLength;
	 std::uint32_t m_flC4Blow;
	 std::uint32_t m_nBombSite;
	 std::uint32_t m_bBombTicking;
	 std::uint32_t m_MeshGroupMask;
	 std::uint32_t m_hModel;
	 std::uint32_t m_pOwner;
	 std::uint32_t m_angRotation;
	 std::uint32_t m_angAbsRotation;
	 std::uint32_t m_pParent;
	 std::uint32_t m_bDormant;
	 std::uint32_t m_vecAbsOrigin;
	 std::uint32_t m_vecOrigin;
	 std::uint32_t m_iAccountID;
	 std::uint32_t m_iItemIDHigh;
	 std::uint32_t m_iItemIDLow;
	 std::uint32_t m_iItemID;
	 std::uint32_t m_iItemDefinitionIndex;
	 std::uint32_t m_AttributeManager;
	 std::uint32_t m_hWeapon;
	 std::uint32_t m_iShotsFired;
	 std::uint32_t m_flLastSmokeOverlayAlpha;
	 std::uint32_t m_flFlashDuration;
	 std::uint32_t m_hOriginalController;
	 std::uint32_t m_aimPunchAngle;
	 std::uint32_t m_bGunGameImmunity;
	 std::uint32_t m_ArmorValue;
	 std::uint32_t m_bIsWalking;
	 std::uint32_t m_bIsScoped;
	 std::uint32_t m_pViewModelServices;
	 std::uint32_t m_pPingServices;
	 std::uint32_t m_pActionTrackingServices;
	 std::uint32_t m_aimPunchCache;
	 std::uint32_t m_pBulletServices;
	 std::uint32_t m_pInventoryServices;
	 std::uint32_t m_sSanitizedPlayerName;
	 std::uint32_t m_hObserverPawn;
	 std::uint32_t m_hPlayerPawn;
	 std::uint32_t m_usSolidFlags;
	 std::uint32_t m_vecMaxs;
	 std::uint32_t m_vecMins;
	 std::uint32_t m_nAlpha;
	 std::uint32_t m_iClip2;
	 std::uint32_t m_iClip1;
	 std::uint32_t m_flNextSecondaryAttackTickRatio;
	 std::uint32_t m_nNextSecondaryAttackTick;
	 std::uint32_t m_flNextPrimaryAttackTickRatio;
	 std::uint32_t m_nNextPrimaryAttackTick;
	 std::uint32_t m_pMovementServices;
	 std::uint32_t m_pCameraServices;
	 std::uint32_t m_pObserverServices;
	 std::uint32_t m_pItemServices;
	 std::uint32_t m_pWeaponServices;
	 std::uint32_t m_iDesiredFOV;
	 std::uint32_t m_nTickBase;
	 std::uint32_t m_bIsLocalPlayerController;
	 std::uint32_t m_steamID;
	 std::uint32_t m_pClientAlphaProperty;
	 std::uint32_t m_vecViewOffset;
	 std::uint32_t m_nRenderFX;
	 std::uint32_t m_hThrower;
	 std::uint32_t m_DmgRadius;
	 std::uint32_t m_flSimulationTime;
	 std::uint32_t m_flOldSimulationTime;
	 std::uint32_t m_hGroundEntity;
	 std::uint32_t m_hOwnerEntity;
	 std::uint32_t m_pCollision;
	 std::uint32_t m_pGameSceneNode;
	 std::uint32_t m_nActualMoveType;
	 std::uint32_t m_MoveType;
	 std::uint32_t m_vecAbsVelocity;
	 std::uint32_t m_vecVelocity;
	 std::uint32_t m_flSpeed;
	 std::uint32_t m_fFlags;
	 std::uint32_t m_iTeamNum;
	 std::uint32_t m_iEFlags;
	 std::uint32_t m_lifeState;
	 std::uint32_t m_iHealth;
	 std::uint32_t m_flThrowStrength;
	 std::uint32_t m_bThrowAnimating;
	 std::uint32_t m_fDropTime;
	 std::uint32_t m_bRedraw;
	 std::uint32_t m_bJumpThrow;
	 std::uint32_t m_bPinPulled;
	 std::uint32_t m_eThrowStatus;
	 std::uint32_t m_fThrowTime;
	 std::uint32_t m_bSequenceFinished;
	 std::uint32_t m_baseLayer;
	 std::uint32_t m_flPrevCycleForAnimEventDetection;
	 std::uint32_t m_flPrevCycleFromDiscontinuity;
	 std::uint32_t m_bDiscontinuity;
	 std::uint32_t m_bSequenceChangeNetworked;
	 std::uint32_t m_flWeight;
	 std::uint32_t m_flCycle;
	 std::uint32_t m_flPrevCycle;
	 std::uint32_t m_hSequence;
	 std::uint32_t m_PlayerFog;
	 std::uint32_t m_Item;
	 std::uint32_t m_bWaitForNoAttack;
	 std::uint32_t m_nSubclassID;
	 std::uint32_t m_flThrowVelocity;
	 std::uint32_t m_flCycleTime;
	 std::uint32_t m_weaponMode;
	 std::uint32_t m_bSmokeEffectSpawned;
	 std::uint32_t m_pEntity;
	 std::uint32_t m_flags;
	 std::uint32_t m_designerName;
	 std::uint32_t m_arrTrajectoryTrailPoints;
	 std::uint32_t m_arrTrajectoryTrailPointCreationTimes;

	 std::uint32_t m_fGlowColor;
	 std::uint32_t m_iGlowType;
	 std::uint32_t m_iGlowTeam;
	 std::uint32_t m_nGlowRange;
	 std::uint32_t m_nGlowRangeMin;
	 std::uint32_t m_glowColorOverride;
	 std::uint32_t m_bFlashing;
	 std::uint32_t m_flGlowTime;
	 std::uint32_t m_flGlowStartTime;
	 std::uint32_t m_bEligibleForScreenHighlight;
	 std::uint32_t m_bGlowing;
	 std::uint32_t m_hEntity;
	 std::uint32_t m_hOtherEntity;
	 std::uint32_t m_nColor;
};

inline auto g_pSchemaOffset = std::make_unique<CSchemaOffset>();

#endif