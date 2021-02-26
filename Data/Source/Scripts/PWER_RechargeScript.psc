scriptname PWER_RechargeScript extends ReferenceAlias

GlobalVariable property TimeScale auto
Actor property PlayerRef auto
ActorValueInfo _enchantingAVI = none
ActorValueInfo property EnchantingAVI hidden
    ActorValueInfo function get()
        if (!_enchantingAVI)
            _enchantingAVI = ActorValueInfo.GetActorValueInfoByName("Enchanting")
        endif
        return _enchantingAVI
    endfunction
endproperty

PWER_MCM property PWER_MCMQuest auto

float TimeOfLastRecharge

event OnInit()
    TimeOfLastRecharge = Utility.GetCurrentGameTime()
	self.OnUpdateGameTime()
endevent

event OnUpdateGameTime()
	float currentTime = Utility.GetCurrentGameTime()
	float daysElapsed = currentTime - TimeOfLastRecharge

	float basePointsToRecharge = daysElapsed * PWER_MCMQuest.RechargePointsPerDay
	if (basePointsToRecharge >= 1.0)
		if (PWER_MCMQuest.EnabledForPlayer)
			float pointsRecharged = PassiveWeaponEnchantmentRecharging.RechargeAllWeaponsInInventory(PlayerRef, basePointsToRecharge * PWER_MCMQuest.RechargeMultiplierPlayer, PWER_MCMQuest.EnchantingSkillRechargeMultiplier)
			RewardExperience(pointsRecharged)
		endif
		if (PWER_MCMQuest.EnabledForFollowers)
			PassiveWeaponEnchantmentRecharging.RechargeAllWeaponsInFollowerInventory(basePointsToRecharge * PWER_MCMQuest.RechargeMultiplierFollowers, PWER_MCMQuest.EnchantingSkillRechargeMultiplier)
		endif
        TimeOfLastRecharge = currentTime
	endif

	if (PWER_MCMQuest.EnabledForPlayer || PWER_MCMQuest.EnabledForFollowers)
		float delay = TimeScale.GetValue() / 3600.0 * PWER_MCMQuest.RechargeInterval
		if (delay < 0.025)
			delay = 0.025
		endif
		self.RegisterForSingleUpdateGameTime(delay)
	endif
endevent

function RewardExperience(float fPointsRecharged)
	if (fPointsRecharged > 0)
		EnchantingAVI.AddSkillExperience(0.0002 * PWER_MCMQuest.ExperienceMultiplier * fPointsRecharged)
	endif
endfunction
