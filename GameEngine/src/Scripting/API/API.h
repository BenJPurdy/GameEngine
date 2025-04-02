#pragma once
#include "Core/Input.h"

#define SCRIPTAPI extern "C" __declspec(dllexport)


namespace GameEngine
{
	class Entity;
	struct Transform;
	struct Render;
	struct MousePosition;
	enum ComponentType
	{
		Component_ID,
		Component_Tag,
		Component_Transform,
		Component_Camera,
		Component_SpriteRender,
		Component_Script,
		Component_CircleRender,
		Component_Rigidbody2d,
		Component_BoxCollider2d,
		Component_CircleCollider2d,
	};

	using OnStartFn = void(*)(Entity);

	
	SCRIPTAPI void scriptSayHello();
	SCRIPTAPI void scriptLog(int level, const char* msg);
	SCRIPTAPI Transform scriptGetTransform(Entity e);
	SCRIPTAPI void scriptAddForce(Entity e, glm::vec3 f);
	SCRIPTAPI void playSound(int index);
	
	SCRIPTAPI Entity scriptGetEntity(const char*);
	SCRIPTAPI Entity scriptSpawnEntity(Entity, const char*);
	SCRIPTAPI Render scriptGetRender(Entity);
	SCRIPTAPI void scriptSetRender(Entity, Render);


	//Components
	SCRIPTAPI void scriptAddComponent(Entity, ComponentType);
	SCRIPTAPI void scriptAssignScriptName(Entity, const char*);
	SCRIPTAPI void scriptDestroy(Entity);

	//IO
	SCRIPTAPI bool scriptGetKey(KeyCode k);
	SCRIPTAPI bool scriptGetMouse(MouseCode m);
	SCRIPTAPI MousePosition scriptGetMousePos();



}