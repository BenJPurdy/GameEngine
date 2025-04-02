#pragma once
#include "Core/Input.h"

#define SCRIPTAPI extern "C" __declspec(dllexport)


namespace GameEngine
{
	class Entity;
	struct Transform;
	struct Render;

	using OnStartFn = void(*)(Entity);

	
	SCRIPTAPI void scriptSayHello();
	SCRIPTAPI void scriptLog(int level, const char* msg);
	SCRIPTAPI void speak();
	SCRIPTAPI Transform scriptGetTransform(Entity e);
	SCRIPTAPI void scriptAddForce(Entity e, glm::vec3 f);
	SCRIPTAPI void playSound(int index);
	SCRIPTAPI bool scriptGetKey(KeyCode k);
	SCRIPTAPI bool scriptGetMouse(MouseCode m);
	SCRIPTAPI Entity scriptGetEntity(const char*);
	SCRIPTAPI Entity scriptSpawnEntity(Entity, const char*);
	SCRIPTAPI Render scriptGetRender(Entity);
	SCRIPTAPI void scriptSetRender(Entity, Render);



}