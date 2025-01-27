#pragma once

#include "Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace GameEngine
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow*);

		virtual void init() override;
		virtual void swapBuffers() override;

	private:
		GLFWwindow* windowHandle;
	};
}
