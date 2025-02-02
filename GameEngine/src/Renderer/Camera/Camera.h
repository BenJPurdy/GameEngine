#pragma once

#include <glm/glm.hpp>

namespace GameEngine
{
	enum class ProjectionType { Orthographic = 0, Perspective = 1 };

	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& proj) : projection(proj) {}
		virtual ~Camera() = default;

		const glm::mat4& getProjection() const { return projection; }

	protected:
		glm::mat4 projection = glm::mat4(1.0f);
	};
}
