#pragma once

#include <glm/glm.hpp>

namespace GameEngine::Maths
{
	bool decomposeTransform(const glm::mat4&, glm::vec3&, glm::vec3&, glm::vec3&);
}
