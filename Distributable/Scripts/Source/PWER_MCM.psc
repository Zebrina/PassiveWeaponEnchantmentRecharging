scriptname PWER_MCM extends SKI_ConfigBase

Quest property PWER_RechargeQuest auto

bool property EnabledForPlayer auto
bool property EnabledForFollowers auto
float property RechargePointsPerDay auto
float property RechargeInterval auto
float property EnchantingSkillRechargeMultiplier auto
float property ExperienceMultiplier auto

int McmID_EnabledForPlayer
int McmID_EnabledForFollowers
int McmID_RechargeInterval
int McmID_RechargePointsPerDay
int McmID_EnchantingSkillRechargeMultiplier
int McmID_ExperienceMultiplier

int function GetOptionFlags(bool enabled)
	if (enabled)
		return OPTION_FLAG_NONE
	endif
	return OPTION_FLAG_DISABLED
endfunction

event OnConfigInit()
	EnabledForPlayer = PWER_RechargeQuest.IsRunning()
endevent

event OnConfigClose()
	if (EnabledForPlayer && !PWER_RechargeQuest.IsRunning())
		PWER_RechargeQuest.Start()
	elseif (!EnabledForPlayer && PWER_RechargeQuest.IsRunning())
		PWER_RechargeQuest.Stop()
	endif
endevent

event OnPageReset(string page)
	SetTitleText("$pwer_title")

	SetCursorFillMode(TOP_TO_BOTTOM)

	AddHeaderOption("$pwer_generaloptions")

	McmID_EnabledForPlayer = AddToggleOption("$pwer_modenabled", EnabledForPlayer)

	int optionFlags = GetOptionFlags(EnabledForPlayer)

	McmID_EnabledForFollowers = AddToggleOption("$pwer_followersenabled", EnabledForFollowers, optionFlags)
	McmID_RechargeInterval = AddSliderOption("$pwer_scriptdelay", RechargeInterval, "$pwer_seconds", optionFlags)

	SetCursorPosition(1)

	AddHeaderOption("$pwer_rechargeoptions")

	McmID_RechargePointsPerDay = AddSliderOption("$pwer_rechargespeed", RechargePointsPerDay, "$pwer_pointsperday", optionFlags)

	McmID_EnchantingSkillRechargeMultiplier = AddSliderOption("$pwer_enchantingmultiplier", EnchantingSkillRechargeMultiplier, "$pwer_atlevelhundred", optionFlags)

	McmID_ExperienceMultiplier = AddSliderOption("$pwer_experiencemultiplier", ExperienceMultiplier, "{2}x", optionFlags)
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
	elseif (option == McmID_EnchantingSkillRechargeMultiplier)
		EnchantingSkillRechargeMultiplier = value
	elseif (option == McmID_ExperienceMultiplier)
		ExperienceMultiplier = value
	endif
	ForcePageReset()
endevent
