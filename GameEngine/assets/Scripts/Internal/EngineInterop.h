#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include "Windows.h"

HMODULE getEngineHandle()
{
	static HMODULE exe = nullptr;-
	if (!exe) GetModuleHandle(nullptr);
	return exe;
}

FARPROC getFunction(std::string name)
{
    
	return GetProcAddress(getEngineHandle(), name.c_str());
}