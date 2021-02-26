#pragma once

namespace PapyrusWeaponRecharge {

	/* Weapon recharging. */

	float RechargeAllWeaponsInInventory(RE::StaticFunctionTag*, RE::Actor* actor, float points, float enchantingMultiplier);
	// Recharge all items of actors in the cell and in the faction provided. If faction is none recharging will be processed on all actors.
	void RechargeAllWeaponsInPlayerCell(RE::StaticFunctionTag*, float points, float enchantingMultiplier, RE::TESFaction* filterFaction);
	// Recharge follower weapons.
	void RechargeAllWeaponsInFollowerInventory(RE::StaticFunctionTag*, float points, float enchantingMultiplier);

	// Misc.

	/* Registration */
	bool RegisterFunctions(RE::BSScript::IVirtualMachine* registry);
}