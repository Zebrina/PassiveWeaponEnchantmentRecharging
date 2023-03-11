#include "PapyrusWeaponRecharge.h"

using namespace std;

constexpr const char* PLUGINNAME = "Passive Weapon Enchantment Recharging";
constexpr uint32_t PLUGINVERSION = 3;

extern "C" __declspec(dllexport) constinit SKSE::PluginVersionData SKSEPlugin_Version = []() {
	SKSE::PluginVersionData version;
	version.PluginVersion(PLUGINVERSION);
	version.PluginName(PLUGINNAME);
	version.AuthorName("Zebrina");
	version.UsesAddressLibrary(true);
	version.CompatibleVersions({ SKSE::RUNTIME_LATEST });
	return version;
}();

extern "C" __declspec(dllexport) bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* skse) {
	auto path = SKSE::log::log_directory();
	if (path) {
		*path /= "PassiveWeaponEnchantmentRecharging.log"sv;
		auto sink = make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);

		auto log = make_shared<spdlog::logger>("global log"s, move(sink));

#ifndef NDEBUG
		log->set_level(spdlog::level::trace);
		log->flush_on(spdlog::level::trace);
#else
		log->set_level(spdlog::level::info);
		log->flush_on(spdlog::level::info);
#endif

		spdlog::set_default_logger(move(log));
		spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);
	}

	LOG_INFO("Plugin loading...");

	SKSE::Init(skse);

	const SKSE::PapyrusInterface* papyrus = SKSE::GetPapyrusInterface();
	if (!papyrus) {
		LOG_ERROR("Unable to get papyrus interface. Aborting plugin load.");
		return false;
	}

	if (!papyrus->Register(PapyrusWeaponRecharge::RegisterFunctions)) {
		LOG_ERROR("Failed to register papyrus functions. Aborting plugin load.");
		return false;
	}

	LOG_INFO("Plugin loaded successfully.");

	return true;
}
