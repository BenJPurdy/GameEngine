#pragma once

#include "Renderer/GraphicsContext.h"

struct SDL_Window;

namespace GameEngine
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(SDL_Window*);

		virtual void init() override;
		virtual void swapBuffers() override;

	private:
		SDL_Window* windowHandle;
	};
}
