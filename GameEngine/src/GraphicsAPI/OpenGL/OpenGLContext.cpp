#include "GameEngine_PCH.h"
#include "OpenGLContext.h"

#include <gl3w.h>
#include <GLFW/glfw3.h>

namespace GameEngine
{
	OpenGLContext::OpenGLContext(GLFWwindow* handle) :windowHandle(handle) {}

	void OpenGLContext::init()
	{
		glfwMakeContextCurrent(windowHandle);
		int err;
		err = gl3wInit();
		CORE_ASSERT("{0} - Failed to initilaise GL3W", err);

		LOG_TRACE("Displaing all logging levels, trace, info, warn, error, fatal");
		LOG_INFO("Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		LOG_WARN("Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		LOG_ERROR("OpenGL Version: {0}", (const char*)glGetString(GL_VERSION));
		LOG_FATAL("GLSL Version: {0}", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
		

	}

	void OpenGLContext::swapBuffers()
	{
		glfwSwapBuffers(windowHandle);
	}
}