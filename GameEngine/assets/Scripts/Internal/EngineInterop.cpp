#include "EngineInterop.h"
#include "../Entity.h"


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

void log(LogLevel lvl, std::string msg)
{
	LogFn f = (LogFn)getFunction("scriptLog");
	f((int)lvl, msg.c_str());
}

bool GetKeyPressed(Key k)
{
    GetKeyPressedFunc f = (GetKeyPressedFunc)getFunction("scriptGetKey");
	if (f == nullptr) { log(LOG_ERROR, "Failed to find getkeypressed fn"); return false; }
    return f(k);
}

bool GetMousePressed(Mouse m)
{
	GetMousePressedFunc f = (GetMousePressedFunc)getFunction("scriptGetMouse");
	if (f == nullptr) {log(LOG_ERROR, "Failed to find getMousePressed"); return false;}
	return f(m);
}

void playSound(int i)
{
	intFunc f = (intFunc)getFunction("playSound");
	if (f == nullptr) { log(LOG_ERROR, "Failed to find playSound"); return;}
	f(0);
}

Entity getEntity(std::string name)
{
	GetEntityFunc f = (GetEntityFunc)getFunction("scriptGetEntity");
	if (f == nullptr) {log(LOG_ERROR, "failed to find getEntity"); return Entity(0, 0);}
	return f(name.c_str());
}