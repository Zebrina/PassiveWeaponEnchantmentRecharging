#include "WeaponRecharge.h"

#include "skse64/GameData.h"
#include "skse64/GameFormComponents.h"
#include "skse64/GameExtraData.h"
#include "skse64/GameRTTI.h"

//#include <list>
//#include <queue>
//#include <set>
//#include <map>
//#include <algorithm>

#define ENCHANTING_AVID	23
#define RIGHTITEMCHARGE_AVID 64
#define LEFTITEMCHARGE_AVID 82

template <class T>
struct AcceptEqual {
	T t;

	AcceptEqual(T _t) : t(_t) {}

	bool operator()(T u) {
		return u == t;
	}

	bool Accept(T u) {
		return u == t;
	}
};

bool getActorDataList(Actor* actor, EntryDataList** out) {
	if (ExtraContainerChanges* containerChanges = static_cast<ExtraContainerChanges*>(actor->extraData.GetByType(kExtraData_ContainerChanges))) {
		if (EntryDataList* containerDataList = containerChanges->data ? containerChanges->data->objList : nullptr) {
			*out = containerDataList;
			return true;
		}
	}
	return false;
}

struct ObjectWithExtraList {
	TESForm* form;
	BaseExtraList* extra;

	ObjectWithExtraList(TESForm* _form, BaseExtraList* _extra) :
		form(_form), extra(_extra) {}

	bool operator<(const ObjectWithExtraList& other) {
		return form->formID < other.form->formID;
	}
};
enum ForEachResult {
	Continue,
	Abort,
	StartOver,
};

/* Functor(Actor*) */
template <class Functor>
void forEachActorInPlayerCell(Functor& f) {
	ForEachResult lastResult = Continue;

	do {
		if (TESObjectCELL* currentCell = (*g_thePlayer)->parentCell) {
			tArray<TESObjectREFR*>& refList = currentCell->objectList;

			for (UInt32 i = 0; i < refList.count; ++i) {
				if (Actor* actor = DYNAMIC_CAST(refList[i], TESObjectREFR, Actor)) {
					lastResult = f(actor);
					if (lastResult != Continue) {
						break;
					}
				}
			}
		}
	}
	while (lastResult == StartOver);
}
/* bool Functor(Actor*, T*, BaseExtraList*) */
/* If T is TESForm (default) all form types will be processed. */
template <class T = TESForm, class Functor>
void forEachObjectInActorInventory(Actor* actor, Functor& f) {
	ForEachResult lastResult = Continue;
	EntryDataList* containerDataList;

	do {
		if (getActorDataList(actor, &containerDataList)) {
			for (auto i = containerDataList->Begin(); !i.End(); ++i) {
				auto e = i.Get();

				if (e && e->type && (T::kTypeID == 0 || e->type->formType == T::kTypeID)) {
					T* t = static_cast<T*>(e->type);

					if (e->extendDataList) {
						for (auto j = e->extendDataList->Begin(); !j.End(); ++j) {
							lastResult = f(actor, t, j.Get());
							if (lastResult != Continue) {
								break;
							}
						}
					}
					else {
						lastResult = f(actor, t, nullptr);
					}
				}
				if (lastResult != Continue) {
					break;
				}
			}
		}
	}
	while (lastResult == StartOver);
}

void updateActorEquippedWeapon(Actor* actor, TESObjectWEAP* weapon, BaseExtraList* extra) {
	if (extra) {
		if (extra->HasType(kExtraData_WornLeft)) {
			CALL_MEMBER_FN(actor, UpdateWeaponAbility)(weapon, extra, true);
		}
		else if (extra->HasType(kExtraData_Worn)) {
			CALL_MEMBER_FN(actor, UpdateWeaponAbility)(weapon, extra, false);
		}
	}
}
void updateActorEquippedArmor(Actor* actor, TESObjectARMO* armor, BaseExtraList* extra) {
	if (extra && extra->HasType(kExtraData_Worn)) {
		CALL_MEMBER_FN(actor, UpdateArmorAbility)(armor, extra);
	}
}

/* Weapon recharging. */

float getActorEnchantingMultiplier(Actor* actor, float baseEnchantMultiplier) {
	if (baseEnchantMultiplier > 1.0f) {
		return (1.0f + (baseEnchantMultiplier - 1.0f) * actor->actorValueOwner.GetCurrent(ENCHANTING_AVID) / 100.0f);
	}
	return 1.0f;
}

struct RechargeWeapon {
	float pointsToRecharge;
	float baseEnchantingMultiplier;
	float rechargedPoints;

	RechargeWeapon(float _pointsToRecharge, float _baseEnchantingMultiplier) :
		pointsToRecharge(_pointsToRecharge),
		baseEnchantingMultiplier(_baseEnchantingMultiplier),
		rechargedPoints(0.0f) {
	}

	ForEachResult operator()(Actor* actor, TESObjectWEAP* weapon, BaseExtraList* extra) {
		if (extra) {
			float maxCharge = 0.0f;
			if (weapon->enchantable.enchantment) {
				maxCharge = (float)weapon->enchantable.maxCharge;
			} else if (ExtraEnchantment* extraEnchant = static_cast<ExtraEnchantment*>(extra->GetByType(kExtraData_Enchantment))) {
				maxCharge = (float)extraEnchant->maxCharge;
			}

			if (maxCharge > 0.0f) {
				float pointsToRechargeForActor = pointsToRecharge * getActorEnchantingMultiplier(actor, baseEnchantingMultiplier);

				bool wornLeft = extra->HasType(kExtraData_WornLeft);
				bool wornRight = !wornLeft && extra->HasType(kExtraData_Worn);
				if (wornLeft || wornRight) {
					UInt32 actorValueID = wornLeft ? LEFTITEMCHARGE_AVID : RIGHTITEMCHARGE_AVID;

					float itemCharge = actor->actorValueOwner.GetCurrent(actorValueID);

					_DMESSAGE("charge: %.2f / %.2f", itemCharge, maxCharge);

					if (itemCharge < maxCharge) {
						pointsToRechargeForActor = min(maxCharge - itemCharge, pointsToRechargeForActor);
						rechargedPoints += pointsToRechargeForActor;
						actor->actorValueOwner.ModBase(actorValueID, pointsToRechargeForActor);
					}
				} else if (ExtraCharge* extraCharge = static_cast<ExtraCharge*>(extra->GetByType(kExtraData_Charge))) {
					rechargedPoints += min(pointsToRechargeForActor, maxCharge - extraCharge->charge);
					extraCharge->charge = min(maxCharge, extraCharge->charge + pointsToRechargeForActor);

					_DMESSAGE("charge: %.2f / %.2f", extraCharge->charge, maxCharge);

					// Remove extra charge if full.
					if (extraCharge->charge == maxCharge) {
						extra->Remove(kExtraData_Charge, extraCharge);
						_DMESSAGE("extra charge removed");
					}
				}
			}
		}

		return Continue;
	}
};

float WeaponRecharge::RechargeAllWeaponsInInventory(Actor* actor, float points, float enchantingMultiplier, bool requireSoulGem) {
	if (actor && points > 0.0f) {
		RechargeWeapon recharge(points, enchantingMultiplier);

		forEachObjectInActorInventory<TESObjectWEAP>(actor, recharge);

		return recharge.rechargedPoints;
	}
	return 0;
}
void WeaponRecharge::RechargeAllWeaponsInPlayerCell(float points, float enchantingMultiplier, TESFaction* filterFaction, bool requireSoulGem) {
	struct RechargeActorWeapons {
		class IsInFaction : public Actor::FactionVisitor {
		public:
			TESFaction* factionToLookFor;

			IsInFaction(TESFaction* faction) : factionToLookFor(faction) {}

			virtual bool Accept(TESFaction* faction, SInt8 rank) {
				return !(faction == factionToLookFor && rank >= 0);
			}
		};

		TESFaction* filterFaction;
		float pointsToRecharge;
		float baseEnchantingMultiplier;

		RechargeActorWeapons(TESFaction* _filterFaction, float _pointsToRecharge, float _baseEnchantingMultiplier) :
			filterFaction(_filterFaction), pointsToRecharge(_pointsToRecharge), baseEnchantingMultiplier(_baseEnchantingMultiplier) {}

		ForEachResult operator()(Actor* actor) {
			if (filterFaction == nullptr || !(actor->VisitFactions(IsInFaction(filterFaction)))) {
				forEachObjectInActorInventory<TESObjectWEAP>(actor, RechargeWeapon(pointsToRecharge, baseEnchantingMultiplier));
			}

			return Continue;
		}
	};

	if (points > 0) {
		forEachActorInPlayerCell(RechargeActorWeapons(filterFaction, points, enchantingMultiplier));
	}
}
void WeaponRecharge::RechargeAllWeaponsInFollowerInventory(float points, float enchantingMultiplier, bool requireSoulGem) {
	struct RechargeActorWeapons {
		float pointsToRecharge;
		float baseEnchantingMultiplier;

		RechargeActorWeapons(float _pointsToRecharge, float _baseEnchantingMultiplier) :
			pointsToRecharge(_pointsToRecharge), baseEnchantingMultiplier(_baseEnchantingMultiplier) {}

		ForEachResult operator()(Actor* actor) {
			if (actor->flags1 & Actor::kFlags_IsPlayerTeammate) {
				forEachObjectInActorInventory<TESObjectWEAP>(actor, RechargeWeapon(pointsToRecharge, baseEnchantingMultiplier));
			}

			return Continue;
		}
	};

	if (points > 0) {
		forEachActorInPlayerCell(RechargeActorWeapons(points, enchantingMultiplier));
	}
}