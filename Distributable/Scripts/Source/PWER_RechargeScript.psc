scriptname PWER_RechargeScript extends ReferenceAlias

ActorValueInfo property EnchantingAVI auto hidden
GlobalVariable property TimeScale auto
Actor property PlayerRef auto

PWER_MCM property PWER_MCMQuest auto

float TimeOfLastRecharge

function StartScript()
	TimeOfLastRecharge = Utility.GetCurrentGameTime()
	self.OnUpdateGameTime()
endfunction

event OnInit()
	EnchantingAVI = ActorValueInfo.GetActorValueInfoByName("Enchanting")

	StartScript()
endevent

event OnUpdateGameTime()
	float currentTime = Utility.GetCurrentGameTime()
	float daysElapsed = currentTime - TimeOfLastRecharge
	TimeOfLastRecharge = currentTime

	float basePointsToRecharge = daysElapsed * PWER_MCMQuest.RechargePointsPerDay

	if (PWER_MCMQuest.EnabledForPlayer)
		float pointsRecharged = PassiveWeaponEnchantmentRecharging.RechargeAllWeaponsInInventory(PlayerRef, basePointsToRecharge * PWER_MCMQuest.RechargeMultiplierPlayer, PWER_MCMQuest.EnchantingSkillRechargeMultiplier)
		RewardExperience(pointsRecharged)
	endif

	if (PWER_MCMQuest.EnabledForFollowers)
		PassiveWeaponEnchantmentRecharging.RechargeAllWeaponsInFollowerInventory(basePointsToRecharge * PWER_MCMQuest.RechargeMultiplierFollowers, PWER_MCMQuest.EnchantingSkillRechargeMultiplier)
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
	if (iPointsRecharged > 0)
		EnchantingAVI.AddSkillExperience(0.0002 * PWER_MCMQuest.ExperienceMultiplier * fPointsRecharged)
	endif
endfunction
