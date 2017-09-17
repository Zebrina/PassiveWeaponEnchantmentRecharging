// http://www.network-science.de/ascii/ banner3-D

#include "Global.h"

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

void Serialization_Revert(SKSESerializationInterface* serializationInterface)
{
	_MESSAGE("Serialization_Revert begin");

	_MESSAGE("Serialization_Revert end");
}

constexpr UInt32 serializationDataVersion = 1;

void Serialization_Save(SKSESerializationInterface* serializationInterface) {
	_MESSAGE("Serialization_Save begin");

	if (serializationInterface->OpenRecord('DATA', serializationDataVersion)) {


		_MESSAGE("saved");
	}

	_MESSAGE("Serialization_Save end");
}

void Serialization_Load(SKSESerializationInterface* serializationInterface) {
	_MESSAGE("Serialization_Load begin");

	UInt32 type;
	UInt32 version;
	UInt32 length;
	bool error = false;

	while (!error && serializationInterface->GetNextRecordInfo(&type, &version, &length)) {
		if (type == 'DATA') {
			if (version == serializationDataVersion) {


				_MESSAGE("read data");
			}
			else {
				_MESSAGE("version mismatch! read data version is %i, expected %i", version, serializationDataVersion);
				error = true;
			}
		}
		else {
			_MESSAGE("unhandled type %08X", type);
			error = true;
		}
	}

	_MESSAGE("Serialization_Load end");
}

void Messaging_Callback(SKSEMessagingInterface::Message* msg) {
	if (msg->type == SKSEMessagingInterface::kMessage_DataLoaded) {
		_MESSAGE("kMessage_DataLoaded begin");

		DataHandler* d = DataHandler::GetSingleton();

		/*
		if (d) {
			auto& perkList = d->perks;

			for (int i = 0; i < perkList.count; ++i) {
				if (perkList[i]) {
					auto& perkEntryList = perkList[i]->perkEntries;

					for (int j = 0; j < perkEntryList.count; ++j) {
						auto perkEntry = perkEntryList[j];

						perkEntry->
					}
				}
			}
		}
		*/

		_MESSAGE("kMessage_DataLoaded end");
	}
}

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

		bool registration = Global::QueryInterfaces(skse, kInterface_Messaging | kInterface_Serialization | kInterface_Papyrus);

		// ### do not do anything else in this callback
		// ### only fill out PluginInfo and return true/false

		_MESSAGE("SKSEPlugin_Query end");

		// supported runtime version
		return registration;
	}

	bool SKSEPlugin_Load(const SKSEInterface * skse) {
		_MESSAGE("SKSEPlugin_Load begin");

		Global::MessagingInterface->RegisterListener(g_pluginHandle, "SKSE", Messaging_Callback);

		// register callbacks and unique ID for serialization

		// ### this must be a UNIQUE ID, change this and email me the ID so I can let you know if someone else has already taken it
		Global::SerializationInterface->SetUniqueID(g_pluginHandle, 'PWER');

		Global::SerializationInterface->SetRevertCallback(g_pluginHandle, Serialization_Revert);
		Global::SerializationInterface->SetSaveCallback(g_pluginHandle, Serialization_Save);
		Global::SerializationInterface->SetLoadCallback(g_pluginHandle, Serialization_Load);

		Global::PapyrusInterface->Register(PapyrusWeaponRecharge::RegisterFunctions);

		_MESSAGE("SKSEPlugin_Load end");

		return true;
	}

};
