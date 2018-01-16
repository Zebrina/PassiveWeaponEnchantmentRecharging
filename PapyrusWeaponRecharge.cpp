#include "PapyrusWeaponRecharge.h"
#include "WeaponRecharge.h"

#include "skse64/GameForms.h"
#include "skse64/GameReferences.h"
#include "skse64/PapyrusNativeFunctions.h"
#include "skse64/PapyrusVM.h"

float PapyrusWeaponRecharge::RechargeAllWeaponsInInventory(StaticFunctionTag*, Actor* actor, float points, float enchantingMultiplier, bool requireSoulGem) {
	_DMESSAGE("Calling RechargeAllWeaponsInInventory(0x%.8x, %f, %f, %i)", actor->formID, points, enchantingMultiplier, requireSoulGem);
	return WeaponRecharge::RechargeAllWeaponsInInventory(actor, points, enchantingMultiplier, requireSoulGem);
}
void PapyrusWeaponRecharge::RechargeAllWeaponsInPlayerCell(StaticFunctionTag*, float points, float enchantingMultiplier, TESFaction* filterFaction, bool requireSoulGem) {
	_DMESSAGE("Calling RechargeAllWeaponsInPlayerCell(%f, %f, ???, %i)", points, enchantingMultiplier, requireSoulGem);
	WeaponRecharge::RechargeAllWeaponsInPlayerCell(points, enchantingMultiplier, filterFaction, requireSoulGem);
}
void PapyrusWeaponRecharge::RechargeAllWeaponsInFollowerInventory(StaticFunctionTag*, float points, float enchantingMultiplier, bool requireSoulGem) {
	_DMESSAGE("Calling RechargeAllWeaponsInFollowerInventory(%f, %f, %i)", points, enchantingMultiplier, requireSoulGem);
	WeaponRecharge::RechargeAllWeaponsInFollowerInventory(points, enchantingMultiplier, requireSoulGem);
}

bool PapyrusWeaponRecharge::RegisterFunctions(VMClassRegistry* registry) {
	constexpr const char* papyrusClassName = "PassiveWeaponEnchantmentRecharging";

	registry->RegisterFunction(new NativeFunction4<StaticFunctionTag, float, Actor*, float, float, bool>("RechargeAllWeaponsInInventory", papyrusClassName, RechargeAllWeaponsInInventory, registry));
	registry->RegisterFunction(new NativeFunction4<StaticFunctionTag, void, float, float, TESFaction*, bool>("RechargeAllWeaponsInPlayerCell", papyrusClassName, RechargeAllWeaponsInPlayerCell, registry));
	registry->RegisterFunction(new NativeFunction3<StaticFunctionTag, void, float, float, bool>("RechargeAllWeaponsInFollowerInventory", papyrusClassName, RechargeAllWeaponsInFollowerInventory, registry));

	return true;
}