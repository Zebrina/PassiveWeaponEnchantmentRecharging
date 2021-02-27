#include "PapyrusWeaponRecharge.h"
#include "WeaponRecharge.h"

using namespace RE;

float PapyrusWeaponRecharge::RechargeAllWeaponsInInventory(StaticFunctionTag*, Actor* actor, float points, float enchantingMultiplier) {
	LOG_TRACE("calling RechargeAllWeaponsInInventory({:X}, {:f}, {:f})", actor ? actor->formID : 0, points, enchantingMultiplier);
	return WeaponRecharge::RechargeAllWeaponsInInventory(actor, points, enchantingMultiplier);
}
void PapyrusWeaponRecharge::RechargeAllWeaponsInPlayerCell(StaticFunctionTag*, float points, float enchantingMultiplier, TESFaction* filterFaction) {
	LOG_TRACE("calling RechargeAllWeaponsInPlayerCell({:f}, {:f}, ???)", points, enchantingMultiplier);
	WeaponRecharge::RechargeAllWeaponsInPlayerCell(points, enchantingMultiplier, filterFaction);
}
void PapyrusWeaponRecharge::RechargeAllWeaponsInFollowerInventory(StaticFunctionTag*, float points, float enchantingMultiplier) {
	LOG_TRACE("calling RechargeAllWeaponsInFollowerInventory({:f}, {:f})", points, enchantingMultiplier);
	WeaponRecharge::RechargeAllWeaponsInFollowerInventory(points, enchantingMultiplier);
}

bool PapyrusWeaponRecharge::RegisterFunctions(BSScript::IVirtualMachine* registry) {
	constexpr const char* papyrusClassName = "PassiveWeaponEnchantmentRecharging";

	registry->RegisterFunction("RechargeAllWeaponsInInventory", papyrusClassName, RechargeAllWeaponsInInventory);
	registry->RegisterFunction("RechargeAllWeaponsInPlayerCell", papyrusClassName, RechargeAllWeaponsInPlayerCell);
	registry->RegisterFunction("RechargeAllWeaponsInFollowerInventory", papyrusClassName, RechargeAllWeaponsInFollowerInventory);

	return true;
}