#pragma once

struct StaticFunctionTag;
class Actor;
class TESFaction;
class VMClassRegistry;

namespace PapyrusWeaponRecharge {

	/* Weapon recharging. */

	float RechargeAllWeaponsInInventory(StaticFunctionTag*, Actor* actor, float points, float enchantingMultiplier, bool requireSoulGem);
	// Recharge all items of actors in the cell and in the faction provided. If faction is none recharging will be processed on all actors.
	void RechargeAllWeaponsInPlayerCell(StaticFunctionTag*, float points, float enchantingMultiplier, TESFaction* filterFaction, bool requireSoulGem);
	// Recharge follower weapons.
	void RechargeAllWeaponsInFollowerInventory(StaticFunctionTag*, float points, float enchantingMultiplier, bool requireSoulGem);

	// Misc.

	/* Registration */
	bool RegisterFunctions(VMClassRegistry* registry);
}