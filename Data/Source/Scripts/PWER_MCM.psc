scriptname PWER_MCM extends SKI_ConfigBase

Quest property PWER_RechargeQuest auto

bool property EnabledForPlayer = true auto
bool property EnabledForFollowers = false auto
float property RechargeInterval = 30.0 auto
float property RechargePointsPerDay = 600.0 auto
float property RechargeMultiplierPlayer = 1.0 auto
float property RechargeMultiplierFollowers = 1.0 auto
float property EnchantingSkillRechargeMultiplier = 8.0 auto
float property ExperienceMultiplier = 0.5 auto

int McmID_EnabledForPlayer
int McmID_EnabledForFollowers
int McmID_RechargeInterval
int McmID_RechargePointsPerDay
int McmID_RechargeMultiplierPlayer
int McmID_RechargeMultiplierFollowers
int McmID_EnchantingSkillRechargeMultiplier
int McmID_ExperienceMultiplier

int function GetOptionFlags(bool enabled)
	if (enabled)
		return OPTION_FLAG_NONE
	endif
	return OPTION_FLAG_DISABLED
endfunction

event OnConfigInit()
endevent

event OnConfigClose()
	if (PWER_RechargeQuest.IsRunning())
		if (!EnabledForPlayer && !EnabledForFollowers)
			PWER_RechargeQuest.Stop()
		endif
	else
		if (EnabledForPlayer || EnabledForFollowers)
			PWER_RechargeQuest.Start()
		endif
	endif
endevent

event OnPageReset(string page)
	SetTitleText("$pwer_title")

	SetCursorFillMode(TOP_TO_BOTTOM)

	AddHeaderOption("$pwer_generaloptions")

	McmID_EnabledForPlayer = AddToggleOption("$pwer_playerenabled", EnabledForPlayer)
	McmID_EnabledForFollowers = AddToggleOption("$pwer_followersenabled", EnabledForFollowers)
	McmID_RechargeInterval = AddSliderOption("$pwer_scriptdelay", RechargeInterval, "$pwer_seconds")

	SetCursorPosition(1)

	AddHeaderOption("$pwer_rechargeoptions")

	McmID_RechargePointsPerDay = AddSliderOption("$pwer_rechargespeed", RechargePointsPerDay, "$pwer_pointsperday")

	McmID_EnchantingSkillRechargeMultiplier = AddSliderOption("$pwer_enchantingmultiplier", EnchantingSkillRechargeMultiplier, "$pwer_atlevelhundred")

	McmID_ExperienceMultiplier = AddSliderOption("$pwer_experiencemultiplier", ExperienceMultiplier, "{2}x")
endevent

event OnOptionHighlight(int option)
	if (option == McmID_RechargePointsPerDay)
		SetInfoText("$pwer_rechargespeedinfo")
	elseif (option == McmID_EnchantingSkillRechargeMultiplier)
		SetInfoText("$pwer_enchantingmultiplierinfo")
	elseif (option == McmID_ExperienceMultiplier)
		SetInfoText("$pwer_experiencemultiplierinfo")
	endif
endevent

event OnOptionSelect(int option)
	if (option == McmID_EnabledForPlayer)
		EnabledForPlayer = !EnabledForPlayer
		;SetToggleOptionValue(option, EnabledForPlayer)
		ForcePageReset()
	elseif (option == McmID_EnabledForFollowers)
		EnabledForFollowers = !EnabledForFollowers
		SetToggleOptionValue(option, EnabledForFollowers)
	endif
endevent

event OnOptionSliderOpen(int option)
	if (option == McmID_RechargeInterval)
		SetSliderDialogStartValue(RechargeInterval)
		SetSliderDialogDefaultValue(30.0)
		SetSliderDialogRange(5.0, 60.0)
		SetSliderDialogInterval(1.0)
	elseif (option == McmID_RechargePointsPerDay)
		SetSliderDialogStartValue(RechargePointsPerDay)
		SetSliderDialogDefaultValue(600.0)
		SetSliderDialogRange(10.0, 30000.0)
		SetSliderDialogInterval(10.0)
	elseif (option == McmID_RechargeMultiplierPlayer || option == McmID_RechargeMultiplierFollowers)
		if (option == McmID_RechargeMultiplierPlayer)
			SetSliderDialogStartValue(RechargeMultiplierPlayer)
		else
			SetSliderDialogStartValue(RechargeMultiplierFollowers)
		endif
		SetSliderDialogDefaultValue(1.0)
		SetSliderDialogRange(0.1, 10.0)
		SetSliderDialogInterval(0.01)
	elseif (option == McmID_RechargeMultiplierPlayer)
		SetSliderDialogStartValue(RechargeMultiplierPlayer)
		SetSliderDialogDefaultValue(1.0)
		SetSliderDialogRange(0.1, 10.0)
		SetSliderDialogInterval(0.01)
	elseif (option == McmID_EnchantingSkillRechargeMultiplier)
		SetSliderDialogStartValue(EnchantingSkillRechargeMultiplier)
		SetSliderDialogDefaultValue(8.0)
		SetSliderDialogRange(1.0, 20.0)
		SetSliderDialogInterval(0.1)
	elseif (option == McmID_ExperienceMultiplier)
		SetSliderDialogStartValue(ExperienceMultiplier)
		SetSliderDialogDefaultValue(0.5)
		SetSliderDialogRange(0.0, 2.0)
		SetSliderDialogInterval(0.01)
	endif
endevent

event OnOptionSliderAccept(int option, float value)
	if (option == McmID_RechargeInterval)
		RechargeInterval = value
	elseif (option == McmID_RechargePointsPerDay)
		RechargePointsPerDay = value
	elseif (option == McmID_RechargeMultiplierPlayer)
		RechargeMultiplierPlayer = value
	elseif (option == McmID_RechargeMultiplierFollowers)
		RechargeMultiplierFollowers = value
	elseif (option == McmID_EnchantingSkillRechargeMultiplier)
		EnchantingSkillRechargeMultiplier = value
	elseif (option == McmID_ExperienceMultiplier)
		ExperienceMultiplier = value
	endif
	ForcePageReset()
endevent
