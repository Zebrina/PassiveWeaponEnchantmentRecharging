#pragma once

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

#include "spdlog/sinks/basic_file_sink.h"

#define LOG_INFO(fmt, ...) SKSE::log::info(fmt, __VA_ARGS__)
#define LOG_WARN(fmt, ...) SKSE::log::warn(fmt, __VA_ARGS__)
#define LOG_CRIT(fmt, ...) SKSE::log::critical(fmt, __VA_ARGS__)
#define LOG_ERROR(fmt, ...) SKSE::log::error(fmt, __VA_ARGS__)
#ifndef NDEBUG
#define LOG_DEBUG(fmt, ...) SKSE::log::debug(fmt, __VA_ARGS__)
#define LOG_TRACE(fmt, ...) SKSE::log::trace(fmt, __VA_ARGS__)
#else
#define LOG_DEBUG(fmt, ...)
#define LOG_TRACE(fmt, ...)
#endif
