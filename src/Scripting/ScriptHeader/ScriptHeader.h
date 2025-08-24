//Header file to be included in all scripts

#pragma once
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include <string>
#include <glm/glm.hpp>

#include "Scene/Entity.h"
#include "Scene/Components.h"

using namespace GameEngine;


HMODULE getEngineHandle()
{
	static HMODULE exe = nullptr;
	if (!exe) GetModuleHandle(nullptr);
	return exe;
}

FARPROC getFunction(std::string name)
{
	return GetProcAddress(getEngineHandle(), name.c_str());
}
//interface script -> engine (would be in the scripting side)		
glm::vec3 getTransformPosition(Entity e)
{
	//get handle to engine EXE
	//get ptr to engine->script func
	//return the component
	void* func = getFunction("scriptGetTransform");
	return ((glm::vec3(*)(Entity))func)(e);
}

glm::vec3 getTransformRotation(Entity e)
{
	void* f = getFunction("scriptGetTransformRotation");
	return ((glm::vec3(*)(Entity))f)(e);
}

template <typename T>
T getComponent(Entity e);

template<>
TransformComponent getComponent<TransformComponent>(Entity e)
{
	void* f = getFunction("scriptGetTransform");
	return ((TransformComponent(*)(Entity))f)(e);
}

template<>
Rigidbody2dComponent getComponent<Rigidbody2dComponent>(Entity e)
{
	void* f = getFunction("scriptGetRigidbody");
	return ((Rigidbody2dComponent(*)(Entity))f)(e);
}

template<>
BoxCollider2dComponent getComponent<BoxCollider2dComponent>(Entity e)
{
	void* f = getFunction("scriptBoxCollider");
	return ((BoxCollider2dComponent(*)(Entity))f)(e);
}

template<>
CircleCollider2dComponent getComponent<CircleCollider2dComponent>(Entity e)
{
	void* f = getFunction("scriptBoxCollider");
	return ((CircleCollider2dComponent(*)(Entity))f)(e);
}

template<>
IDComponent getComponent<IDComponent>(Entity e)
{
	void* f = getFunction("scriptBoxCollider");
	return ((IDComponent(*)(Entity))f)(e);
}

template<>
TagComponent getComponent<TagComponent>(Entity e)
{
	void* f = getFunction("scriptBoxCollider");
	return ((TagComponent(*)(Entity))f)(e);
}

template<>
CameraComponent getComponent<CameraComponent>(Entity e)
{
	void* f = getFunction("scriptBoxCollider");
	return ((CameraComponent(*)(Entity))f)(e);
}

template<>
SpriteRenderComponent getComponent<SpriteRenderComponent>(Entity e)
{
	void* f = getFunction("scriptBoxCollider");
	return ((SpriteRenderComponent(*)(Entity))f)(e);
}

template<>
CircleRenderComponent getComponent<CircleRenderComponent>(Entity e)
{
	void* f = getFunction("scriptBoxCollider");
	return ((CircleRenderComponent(*)(Entity))f)(e);
}


//getComponent<TransformComponent>(e);

