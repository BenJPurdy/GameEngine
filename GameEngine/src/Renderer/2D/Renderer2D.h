#pragma once
#include "Renderer/Camera/Camera.h"
#include "Renderer/Camera/EditorCamera.h"
#include "Scene/Components.h"

namespace GameEngine
{
	class Render2d
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
		static void drawQuad(const glm::mat4&, const glm::vec4, int entityID = -1);

		static void drawRotatedQuad(const glm::vec2&, const glm::vec2, const float, const glm::vec4);
		static void drawRotatedQuad(const glm::vec3&, const glm::vec2, const float, const glm::vec4);
	
		static void drawSprite(const glm::mat4&, SpriteRenderComponent&, int);

		struct Statistics
		{
			uint32_t drawCalls = 0;
			uint32_t quadCount = 0;
			uint32_t getTotalVertexCount() { return quadCount * 4; }
			uint32_t getTotalIndexCount() { return quadCount * 6; }
		};

		static void resetStats(); 
		static Statistics getStats();

	private:
		static void startBatch();
		static void nextBatch();
	};
}
