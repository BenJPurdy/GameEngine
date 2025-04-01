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

using SetTransformFunc = void(*)(Entity, Transform);
using GetTransformFunc = Transform(*)(Entity);
using AddForceFunc = void(*)(Entity, glm::vec3);


