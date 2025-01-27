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

		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int w, int h)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				data.width = w;
				data.height = h;

				WindowResizeEvent ev(w, h);

				data.eventCallback(ev);
			});

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent ev;
				data.eventCallback(ev);
			});
		
		glfwSetKeyCallback(window, [](GLFWwindow* window, int k, int s, int a, int m)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				switch (a)
				{
				case GLFW_PRESS:
					KeyPressedEvent e(k, 0);
					data.eventCallback(e);
					break;
				case GLFW_RELEASE:
					KeyReleasedEvent e(k);
					data.eventCallback(e);
					break;
				case GLFW_REPEAT:
					KeyPressedEvent e(k, 1);
					data.eventCallback(e);
					break;
				}
			});

		glfwSetCharCallback(window, [](GLFWwindow* w, uint32_t k)
			{
				WindowData& d = *(WindowData*)glfwGetWindowUserPointer(w);
				KeyTypedEvent e(k);
				d.eventCallback(e);
			});
		
		glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int b, int a, int m)
			{
				WindowData& d = *(WindowData*)glfwGetWindowUserPointer(w);
				switch (a)
				{
				case GLFW_PRESS:
					MouseButtonPressedEvent e(b);
					d.eventCallback(e);
					break;
				case GLFW_RELEASE:
					MouseButtonReleasedEvent e(b);
					d.eventCallback(e);
					break;
				}
			});

		glfwSetScrollCallback(window, [](GLFWwindow* w, double x, double y)
			{
				WindowData& d = *(WindowData*)glfwGetWindowUserPointer(w);
				MouseMovedEvent e((float)x, (float)y);
				d.eventCallback(e);
			});

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