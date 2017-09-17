// http://www.network-science.de/ascii/ banner3-D

#include "skse64/PluginAPI.h"
#include "skse64_common/skse_version.h"
#include "skse64/GameAPI.h"

#include "skse64/GameData.h"
#include "skse64/GameReferences.h"
#include "skse64/GameEvents.h"
#include "skse64/GameTypes.h"

#include "skse64/PapyrusNativeFunctions.h"

#include "skse64_common/SafeWrite.h"

#include "PapyrusWeaponRecharge.h"

constexpr char* PluginName = "Passive Weapon Enchantment Recharging";

IDebugLog gLog("PassiveWeaponEnchantmentRecharging.log");

PluginHandle g_pluginHandle = kPluginHandle_Invalid;

SKSEPapyrusInterface* g_papyrusInterface = nullptr;

extern "C" {

	bool SKSEPlugin_Query(const SKSEInterface* skse, PluginInfo* info) {
		_MESSAGE("SKSEPlugin_Query begin");

		// populate info structure
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = PluginName;
		info->version = 1;

		// store plugin handle so we can identify ourselves later
		g_pluginHandle = skse->GetPluginHandle();

		if (skse->isEditor) {
			_MESSAGE("loaded in editor, marking as incompatible");

			return false;
		}
		else if (skse->runtimeVersion != RUNTIME_VERSION_1_4_2) {
			_MESSAGE("unsupported runtime version %08X", skse->runtimeVersion);

			return false;
		}

		g_papyrusInterface = (SKSEPapyrusInterface*)skse->QueryInterface(kInterface_Papyrus);
		if (!g_papyrusInterface) {
			_MESSAGE("\tcouldn't get papyrus interface");

			return false;
		}
		if (g_papyrusInterface->interfaceVersion < SKSEPapyrusInterface::kInterfaceVersion) {
			_MESSAGE("\tpapyrus interface too old (%d expected %d)", g_papyrusInterface->interfaceVersion, SKSEPapyrusInterface::kInterfaceVersion);

			return false;
		}

		// ### do not do anything else in this callback
		// ### only fill out PluginInfo and return true/false

		_MESSAGE("SKSEPlugin_Query end");

		// supported runtime version
		return true;
	}

	bool SKSEPlugin_Load(const SKSEInterface* skse) {
		_MESSAGE("SKSEPlugin_Load begin");

		g_papyrusInterface->Register(PapyrusWeaponRecharge::RegisterFunctions);

		_MESSAGE("SKSEPlugin_Load end");

		return true;
	}

};
