#include "Global.h"

#include "skse64/PluginAPI.h"

SKSEMessagingInterface*			Global::MessagingInterface;
SKSESerializationInterface*		Global::SerializationInterface;
SKSETaskInterface*				Global::TaskInterface;
SKSEPapyrusInterface*			Global::PapyrusInterface;
SKSEScaleformInterface*			Global::ScaleformInterface;

bool Global::QueryInterfaces(const SKSEInterface* skse, UInt32 interfaces) {
	if (interfaces & kInterface_Messaging) {
		// get the messaging interface and query its version
		Global::MessagingInterface = (SKSEMessagingInterface*)skse->QueryInterface(kInterface_Messaging);
		if (!Global::MessagingInterface) {
			_MESSAGE("\tcouldn't get messaging interface");

			return false;
		}
		if (Global::MessagingInterface->kInterfaceVersion < SKSEMessagingInterface::kInterfaceVersion) {
			_MESSAGE("\tmessaging interface too old (%d expected %d)", Global::MessagingInterface->interfaceVersion, SKSEMessagingInterface::kInterfaceVersion);

			return false;
		}
	}

	if (interfaces & kInterface_Serialization) {
		// get the task interface and query its version
		Global::SerializationInterface = (SKSESerializationInterface*)skse->QueryInterface(kInterface_Serialization);
		if (!Global::SerializationInterface) {
			_MESSAGE("\tcouldn't get serialization interface");

			return false;
		}
		if (Global::SerializationInterface->version < SKSESerializationInterface::kVersion) {
			_MESSAGE("\tserialization interface too old (%d expected %d)", Global::SerializationInterface->version, SKSESerializationInterface::kVersion);

			return false;
		}
	}

	if (interfaces & kInterface_Task) {
		// get the task interface and query its version
		Global::TaskInterface = (SKSETaskInterface*)skse->QueryInterface(kInterface_Task);
		if (!Global::TaskInterface) {
			_MESSAGE("\tcouldn't get task interface");

			return false;
		}
		if (Global::TaskInterface->kInterfaceVersion < SKSETaskInterface::kInterfaceVersion) {
			_MESSAGE("\ttask interface too old (%d expected %d)", Global::TaskInterface->interfaceVersion, SKSETaskInterface::kInterfaceVersion);

			return false;
		}
	}

	if (interfaces & kInterface_Papyrus) {
		// get the papyrus interface and query its version
		Global::PapyrusInterface = (SKSEPapyrusInterface*)skse->QueryInterface(kInterface_Papyrus);
		if (!Global::PapyrusInterface) {
			_MESSAGE("\tcouldn't get papyrus interface");

			return false;
		}
		if (Global::PapyrusInterface->interfaceVersion < SKSEPapyrusInterface::kInterfaceVersion) {
			_MESSAGE("\tpapyrus interface too old (%d expected %d)", Global::PapyrusInterface->interfaceVersion, SKSEPapyrusInterface::kInterfaceVersion);

			return false;
		}
	}

	if (interfaces & kInterface_Scaleform) {
		// get the papyrus interface and query its version
		Global::ScaleformInterface = (SKSEScaleformInterface*)skse->QueryInterface(kInterface_Scaleform);
		if (!Global::ScaleformInterface) {
			_MESSAGE("\tcouldn't get scaleform interface");

			return false;
		}
		if (Global::ScaleformInterface->interfaceVersion < SKSEScaleformInterface::kInterfaceVersion) {
			_MESSAGE("\tscaleform interface too old (%d expected %d)", Global::ScaleformInterface->interfaceVersion, SKSEScaleformInterface::kInterfaceVersion);

			return false;
		}
	}

	return true;
}