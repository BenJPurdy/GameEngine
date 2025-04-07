#include "EngineInterop.h"
#include "../Entity.h"
std::unordered_map<uint32_t, void*> entityData;
void log(LogLevel, std::string);

HMODULE getEngineHandle()
{
	HMODULE exe = nullptr;
	if (!exe) exe = GetModuleHandle(nullptr);
	return exe;
}

FARPROC getFunction(std::string name)
{
	FARPROC f = GetProcAddress(getEngineHandle(), name.c_str());
	if (f == nullptr) { std::string msg = "failed to get fn: " + name; log(LOG_ERROR, msg); return nullptr;}
	return f;
}

void log(LogLevel lvl, std::string msg)
{
	std::string scirptMsg = "SCRIPTING: " + msg;
	LogFn f = (LogFn)getFunction("scriptLog");
	f((int)lvl, scirptMsg.c_str());
}

bool GetKeyPressed(Key k)
{
    GetKeyPressedFunc f = (GetKeyPressedFunc)getFunction("scriptGetKey");
	if (f ==  nullptr) return false;
    return f(k);
}
 
bool GetMousePressed(Mouse m)
{
	GetMousePressedFunc f = (GetMousePressedFunc)getFunction("scriptGetMouse");
	if (f == nullptr) return false;
	return f(m);
}

void playSound(int i)
{
	intFunc f = (intFunc)getFunction("playSound");
	if (f == nullptr) return;
	f(0);
}

Entity getEntity(std::string name)
{
	GetEntityFunc f = (GetEntityFunc)getFunction("scriptGetEntity");
	if (f == nullptr) return Entity(0, 0);
	return f(name.c_str());
}

//Spawned entities inheret their spawning entities rigidbody type
Entity spawnEntity(Entity e, std::string name)
{
	SpawnEntityFunc f = (SpawnEntityFunc)getFunction("scriptSpawnEntity");
	if (f == nullptr) return Entity(0, 0);
	return f(e, name.c_str());
}

MousePosition GetMousePos()
{
    GetMousePosFunc f = (GetMousePosFunc)getFunction("scriptGetMousePos");
    if (f == nullptr) return MousePosition{};
	glm::vec2 mPos = f();
	
	
    return MousePosition{mPos};
}

