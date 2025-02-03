#pragma once

#include <gl3w.h>

namespace GameEngine
{
	class Render3d
	{
	public:
		Render3d() {};
		~Render3d() = default;

		void preProcessing();

		void renderTriangle();
		void setupTriangle();

	private:
		uint32_t triVAO, triVBO, triIBO;
	};
}