#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include <string>

HMODULE getEngineHandle()
{
	HMODULE exe = nullptr;
	if (!exe) exe = GetModuleHandle(nullptr);
	return exe;
}

FARPROC getFunction(std::string name)
{
	return GetProcAddress(getEngineHandle(), name.c_str());
}