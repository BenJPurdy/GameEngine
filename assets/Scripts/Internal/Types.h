#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"

class Entity;
struct Transform
{
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	Transform()
	{
		position = glm::vec3{0}; rotation = glm::vec3{0}; scale = glm::vec3{1};
	}
	std::string to_string()
	{
		return "P: " +  glm::to_string(position) + " R: " + glm::to_string(rotation) + " S: " + glm::to_string(scale);
	}

};

enum ColliderType
{
	ColliderType_None,
	ColliderType_Box,
	ColliderType_Circle
};

struct ColliderProperties
{
	//ColliderType type;
	glm::vec2 offset = glm::vec2(0);
	float density = 1.0f;
	float friction = 0.5f;
	float restitution = 0.5f;
	glm::vec2 scale = glm::vec2(1);


};

 enum PhysicsProperties : uint16_t
 {
     PhysProps_None = 0,
     PhysProps_Static = 0x1,         //BIT(1)
     PhysProps_Dynamic = 0x2,        //BIT(2)
     PhysProps_Kinematic = 0x4,      //BIT(3)
     PhysProps_FixedRotation = 0x8   //BIT(4)
 };

  struct RigidbodyProperties
 {
     uint16_t properties = PhysicsProperties::PhysProps_None;
     float mass;
     bool has(uint16_t c) { return (properties & c) == c; };
     void set(uint16_t c) { properties |= c; }
     uint16_t getBodyType() { return properties & 0b00000111; }
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
using SetColliderPropertiesFunc = void(*)(Entity, ColliderProperties);
using CopyRigidbodyPropsFunc = void(*)(Entity, Entity);
using SetRigidbodyPropsFunc = void(*)(Entity, RigidbodyProperties);


