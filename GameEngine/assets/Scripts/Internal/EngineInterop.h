#pragma once
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include <cstdint>
#include <string>
#include "Keys.h"
#include "glm/glm.hpp"

#define ACCESSPOINT extern "C"
#define SCRIPT __declspec(dllexport)
#define SCRIPTEXPORT __attribute__ ((dllexport))

#define SCRIPTAPI ACCESSPOINT SCRIPT

class Entity;
using LogFn = void(*)(int, const char*);
using GetKeyPressedFunc = bool(*)(Key);
using GetMousePressedFunc = bool(*)(Mouse);
using GetEntityFunc = Entity(*)(const char*);
using SpawnEntityFunc = Entity(*)(Entity, const char*);
using VoidEntityFunc = void(*)(Entity);


using voidFunc = void(*)();
using intFunc =  void(*)(int);

struct MousePosition
{
	float x;
	float y;

	MousePosition(glm::vec2 in): x(in.x), y(in.y){}
	MousePosition() = default;
};




enum LogLevel
{
	LOG_TRACE,
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR,
	LOG_FATAL
};

HMODULE getEngineHandle();
FARPROC getFunction(std::string name);
void log(LogLevel lvl, std::string msg);
bool GetKeyPressed(Key k);
bool GetMousePressed(Mouse m);
void playSound(int i);
Entity getEntity(std::string name);
Entity spawnEntity(Entity, std::string);
MousePosition GetMousePos();