scriptname PWER_RechargeScript extends ReferenceAlias

ActorValueInfo property EnchantingAVI auto
GlobalVariable property TimeScale auto
Actor property PlayerRef auto

PWER_MCM property PWER auto

float TimeOfLastRecharge

function StartScript()
	TimeOfLastRecharge = Utility.GetCurrentGameTime()
	RechargeBuffer = 0.0
	self.OnUpdateGameTime()
endfunction

event OnInit()
	EnchantingAVI = GetActorValueInfoByName("Enchanting")

	StartScript()
endevent

event OnUpdateGameTime()
	currentTime = Utility.GetCurrentGameTime()
	float daysElapsed = currentTime - TimeOfLastRecharge
	TimeOfLastRecharge = currentTime

	float basePointsToRecharge = daysElapsed * PWER.RechargePointsPerDay

	if (PWER.EnabledForPlayer)
		float pointsRecharged = PassiveWeaponEnchantmentRecharging.RechargeAllWeaponsInInventory(PlayerRef, basePointsToRecharge, , PWER.EnchantingSkillRechargeMultiplier)
		RewardExperience(pointsRecharged)
	endif

	if (PWER.EnabledForFollowers)
		PassiveWeaponEnchantmentRecharging.RechargeAllWeaponsInFollowerInventory(basePointsToRecharge, PWER.EnchantingSkillRechargeMultiplier)
	endif

	if (PWER.EnabledForPlayer || PWER.EnabledForFollowers)
		float delay = TimeScale.GetValue() / 3600.0 * PWER.RechargeInterval
		if (delay < 0.025)
			delay = 0.025
		endif
		self.RegisterForSingleUpdateGameTime(delay)
	endif
endevent

function RewardExperience(int iPointsRecharged)
	if (iPointsRecharged > 0)
		EnchantingAVI.AddSkillExperience(0.0002 * PWER.ExperienceMultiplier * (iPointsRecharged as float))
	endif
endfunction
