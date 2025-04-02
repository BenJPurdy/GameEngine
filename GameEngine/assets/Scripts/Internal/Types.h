#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"

class Entity;
struct Transform
{
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	std::string to_string()
	{
		return "P: " +  glm::to_string(position) + " R: " + glm::to_string(rotation) + " S: " + glm::to_string(scale);
	}

};

struct Render
{
	glm::vec4 colour;
	union
	{
		glm::vec2 extents;
		float radius;
	};
};

enum ComponentType
{
	IDComponent,
	TagComponent,
	TransformComponent,
	CameraComponent,
	SpriteRenderComponent,
	ScriptComponent,
	CircleRenderComponent,
	Rigidbody2dComponent,
	BoxCollider2dComponent,
	CircleCollider2dComponent,
};

using GetRenderFunc = Render(*)(Entity);
using SetRenderFunc = void(*)(Entity, Render);
using SetTransformFunc = void(*)(Entity, Transform);
using GetTransformFunc = Transform(*)(Entity);
using AddForceFunc = void(*)(Entity, glm::vec3);
using AddComponentFunc = bool(*)(Entity, ComponentType);
using DestroyFunc = void(*)(Entity);
using SetScriptNameFunc = void(*)(Entity, const char*);
using GetMousePosFunc = glm::vec2(*)();


