#include "GameEngine_PCH.h"
#include "WindowsWindow.h"

#include "GraphicsAPI/OpenGL/OpenGLContext.h"

namespace GameEngine
{
	static uint8_t windowCount = 0;

	static void WindowErrorCallback(int errorCode, const char* errorMessage)
	{
		std::cout << "GLFW_ERROR: [" << errorCode << "] - " << errorMessage << std::endl;
		__debugbreak();
	}

	std::unique_ptr<Window> Window::create(const WindowProps& props)
	{
		return std::make_unique<WindowsWindow>(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& p)
	{
		init(p);
	}

	WindowsWindow::~WindowsWindow() { shutdown(); }

	void WindowsWindow::init(const WindowProps& p)
	{
		data.title = p.title;
		data.width = p.w;
		data.height = p.h;

		if (windowCount == 0)
		{
			auto err = glfwInit();
			glfwSetErrorCallback(WindowErrorCallback);
		}

		window = glfwCreateWindow((int)p.w, (int)p.h, data.title.c_str(), nullptr, nullptr);
		windowCount++;

		glfwSetWindowUserPointer(window, &data);

		setVSync(true);

		context = new OpenGLContext(window);
		context->init();

		
		setVSync(true);

		
		
	}

	void WindowsWindow::shutdown()
	{
		glfwDestroyWindow(window);
		--windowCount;
		if (windowCount == 0)
		{
			glfwTerminate();
		}
	}

	void WindowsWindow::onUpdate()
	{
		glfwPollEvents();

		context->swapBuffers();
	}

	void WindowsWindow::setVSync(bool enabled) { data.vSync = enabled; }
	bool WindowsWindow::isVSync() const { return data.vSync; }
}