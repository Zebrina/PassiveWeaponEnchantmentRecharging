#pragma once

namespace WeaponRecharge {
	// Recharge all weapons in the player's inventory.
	float RechargeAllWeaponsInInventory(RE::Actor* actor, float points, float enchantingMultiplier);
	// Recharge all items of actors in the same cell as the player and in the faction provided. If faction is none recharging will be processed on all actors.
	void RechargeAllWeaponsInPlayerCell(float points, float enchantingMultiplier, RE::TESFaction* filterFaction);
	// Recharge all follower's weapons.
	void RechargeAllWeaponsInFollowerInventory(float points, float enchantingMultiplier);
}