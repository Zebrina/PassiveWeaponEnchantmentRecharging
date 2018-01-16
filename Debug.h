#pragma once

#ifdef _DEBUG
#include "skse64/GameAPI.h"

#define _DMESSAGE(fmt, ...) _MESSAGE(fmt, __VA_ARGS__);Console_Print(fmt, __VA_ARGS__)
#else
#define _DMESSAGE(...)
#endif