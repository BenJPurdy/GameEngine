#include "GameEngine_PCH.h"
#include "OpenGLContext.h"

#include <gl3w.h>
#include <GLFW/glfw3.h>

namespace GameEngine
{
	OpenGLContext::OpenGLContext(GLFWwindow* handle) :windowHandle(handle) {}

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
		glfwSwapBuffers(windowHandle);
	}
}