#include "WeaponRecharge.h"

using namespace RE;

bool GetActorInventoryDataList(Actor* actor, BSSimpleList<InventoryEntryData*>*& out) {
	if (ExtraContainerChanges* containerChanges = static_cast<ExtraContainerChanges*>(actor->extraList.GetByType(ExtraDataType::kContainerChanges))) {
		if (BSSimpleList<InventoryEntryData*>* containerDataList = containerChanges->changes ? containerChanges->changes->entryList : nullptr) {
			out = containerDataList;
			return true;
		}
	}
	return false;
}

enum ForEachResult {
	Continue,
	Abort,
	StartOver,
};

/* Functor(Actor*) */
template <class Functor>
void ForEachActorInPlayerCell(Functor f) {
	ForEachResult lastResult = Continue;

	do {
		if (TESObjectCELL* currentCell = PlayerCharacter::GetSingleton()->parentCell) {
			BSTArray<TESObjectREFR*>& refList = currentCell->objectList;

			for (auto i = refList.begin(); i != refList.end(); ++i) {
				TESObjectREFR* ref = *i;
				if (ref->formType == FormType::ActorCharacter) {
					lastResult = f(static_cast<Actor*>(ref));
					if (lastResult != Continue) {
						break;
					}
				}
			}
		}
	}
	while (lastResult == StartOver);
}
/* ForEachResult Functor(Actor*, TESBoundObject*, ExtraDataList*) */
template <class Functor>
void ForEachObjectInActorInventory(Actor* actor, Functor f) {
	ForEachResult lastResult = Continue;
	BSSimpleList<InventoryEntryData*>* containerDataList;

	do {
		if (GetActorInventoryDataList(actor, containerDataList)) {
			for (auto i = containerDataList->begin(); i != containerDataList->end(); ++i) {
				InventoryEntryData* e = *i;

				if (e->extraLists) {
					for (auto j = e->extraLists->begin(); j != e->extraLists->end(); ++j) {
						lastResult = f(actor, e->object, *j);
						if (lastResult != Continue) {
							break;
						}
					}
				}
				else {
					lastResult = f(actor, e->object, nullptr);
				}

				if (lastResult != Continue) {
					break;
				}
			}
		}
	}
	while (lastResult == StartOver);
}

/* Weapon recharging. */

float GetActorEnchantingMultiplier(Actor* actor, float baseEnchantMultiplier) {
	if (baseEnchantMultiplier > 1.0f) {
		return (1.0f + (baseEnchantMultiplier - 1.0f) * actor->GetActorValue(ActorValue::kEnchanting) / 100.0f);
	}
	return 1.0f;
}

struct RechargeWeapon {
	float pointsToRecharge;
	float baseEnchantingMultiplier;
	float* rechargedPoints;

	RechargeWeapon(float _pointsToRecharge, float _baseEnchantingMultiplier, float& _rechargedPoints) :
		pointsToRecharge(_pointsToRecharge),
		baseEnchantingMultiplier(_baseEnchantingMultiplier),
		rechargedPoints(&_rechargedPoints) {
	}
	RechargeWeapon(float _pointsToRecharge, float _baseEnchantingMultiplier) :
		pointsToRecharge(_pointsToRecharge),
		baseEnchantingMultiplier(_baseEnchantingMultiplier),
		rechargedPoints(nullptr) {
	}

	ForEachResult operator()(Actor* actor, TESBoundObject* object, ExtraDataList* extra) {
		if (object->formType == FormType::Weapon && extra) {
			TESObjectWEAP* weapon = static_cast<TESObjectWEAP*>(object);

			float maxCharge = 0.0f;
			if (weapon->formEnchanting) {
				maxCharge = (float)weapon->amountofEnchantment;
			} else if (ExtraEnchantment* extraEnchant = static_cast<ExtraEnchantment*>(extra->GetByType(ExtraDataType::kEnchantment))) {
				maxCharge = (float)extraEnchant->charge;
			}

			if (maxCharge > 0.0f) {
				float pointsToRechargeForActor = pointsToRecharge * GetActorEnchantingMultiplier(actor, baseEnchantingMultiplier);

				bool wornLeft = extra->HasType(ExtraDataType::kWornLeft);
				bool wornRight = !wornLeft && extra->HasType(ExtraDataType::kWorn);
				if (wornLeft || wornRight) {
					ActorValue actorValue = wornLeft ? ActorValue::kLeftItemCharge : ActorValue::kRightItemCharge;

					float itemCharge = actor->GetActorValue(actorValue);

					LOG_TRACE("charge: {:.2f} / {:.2f}", itemCharge, maxCharge);

					if (itemCharge < maxCharge) {
						pointsToRechargeForActor = std::min(maxCharge - itemCharge, pointsToRechargeForActor);
						if (rechargedPoints) {
							*rechargedPoints += pointsToRechargeForActor;
						}
						actor->ModActorValue(actorValue, pointsToRechargeForActor);
					}
				} else if (ExtraCharge* extraCharge = static_cast<ExtraCharge*>(extra->GetByType(ExtraDataType::kCharge))) {
					if (rechargedPoints) {
						*rechargedPoints += std::min(pointsToRechargeForActor, maxCharge - extraCharge->charge);
					}
					extraCharge->charge = std::min(maxCharge, extraCharge->charge + pointsToRechargeForActor);

					LOG_TRACE("charge: {:.2f} / {:.2f}", extraCharge->charge, maxCharge);

					// Remove extra charge if full.
					if (extraCharge->charge == maxCharge) {
						extra->Remove(ExtraDataType::kCharge, extraCharge);

						LOG_TRACE("extra charge removed");
					}
				}
			}
		}

		return Continue;
	}
};

float WeaponRecharge::RechargeAllWeaponsInInventory(Actor* actor, float points, float enchantingMultiplier) {
	if (actor && points > 0.0f) {
		float rechargedPoints = 0.0f;

		ForEachObjectInActorInventory(actor, RechargeWeapon(points, enchantingMultiplier, rechargedPoints));

		return rechargedPoints;
	}
	return 0.0f;
}
void WeaponRecharge::RechargeAllWeaponsInPlayerCell(float points, float enchantingMultiplier, TESFaction* filterFaction) {
	if (points > 0.0f) {
		ForEachActorInPlayerCell([filterFaction, points, enchantingMultiplier](Actor* actor)->ForEachResult {
			if (filterFaction == nullptr || !(actor->IsInFaction(filterFaction))) {
				ForEachObjectInActorInventory(actor, RechargeWeapon(points, enchantingMultiplier));
			}

			return Continue;
		});
	}
}
void WeaponRecharge::RechargeAllWeaponsInFollowerInventory(float points, float enchantingMultiplier) {
	if (points > 0.0f) {
		ForEachActorInPlayerCell([points, enchantingMultiplier](Actor* actor)->ForEachResult {
			if (actor->IsPlayerTeammate()) {
				ForEachObjectInActorInventory(actor, RechargeWeapon(points, enchantingMultiplier));
			}

			return Continue;
		});
	}
}