#include "GameEngine_PCH.h"
#include "OpenGLContext.h"

#include <gl3w.h>
#include <SDL.h>

namespace GameEngine
{
	OpenGLContext::OpenGLContext(SDL_Window* handle) :windowHandle(handle) {}

	void OpenGLContext::init()
	{
		int err;
		err = gl3wInit();
		if (err != GL3W_OK)
		{
			std::cout << "failed to initialise openGL (GL3W_ERR " << err << std::endl;
		}

	}

	void OpenGLContext::swapBuffers()
	{
		SDL_GL_SwapWindow(windowHandle);
	}
}