#pragma once

#include "GameEngine_PCH.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GameEngine
{
	struct QuadVertex
	{
		glm::vec3 pos;
		glm::vec4 colour;
		glm::vec2 texCoord;
		glm::vec2 tiling;
	};
}