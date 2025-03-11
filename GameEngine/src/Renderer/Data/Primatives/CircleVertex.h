#pragma once
#include "GameEngine_PCH.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GameEngine
{
	struct CircleVertex
	{
		glm::vec3 worldPos;
		glm::vec3 localPos;
		glm::vec4 colour;
		float thickness;
		float fade;

		int eID;
	};
}
