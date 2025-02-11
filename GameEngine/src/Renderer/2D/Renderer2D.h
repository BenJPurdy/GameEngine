#pragma once
#include "Renderer/Camera/Camera.h"
#include "Renderer/Camera/EditorCamera.h"

namespace GameEngine
{
	class Renderer2D
	{
	public:
		static void init();
		static void shutdown();

		static void clear();
		static void setClearColour(const glm::vec4&);

		static void beginScene(const EditorCamera&);
		static void beginScene(const Camera&, const glm::mat4&);
		static void endScene();

		static void flush();

		static void drawQuad(const glm::vec2&, const glm::vec2, const glm::vec4);
		static void drawQuad(const glm::vec3&, const glm::vec2, const glm::vec4);
		static void drawQuad(const glm::mat4&, const glm::vec4);

		static drawRotatedQuad(const glm::vec2&, const glm::vec2, const float);
		static drawRotatedQuad(const glm::vec3&, const glm::vec2, const float);
	};
}
