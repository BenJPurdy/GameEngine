#include "GameEngine_PCH.h"
#include "WindowsWindow.h"

namespace GameEngine
{
	static uint8_t windowCount = 0;

	static void WindowErrorCallback(int errorCode, const char* errorMessage)
	{
		std::cout << "SDL_ERROR: [" << errorCode << "] - " << errorMessage << std::endl;
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
			auto err = SDL_Init(SDL_INIT_VIDEO);
			if (err != 0) { WindowErrorCallback(err, SDL_GetError()); }
		}

		window = SDL_CreateWindow("HSIAGE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			data.width, data.height,
			SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		windowCount++;

		SDL_SetWindowData(window, "data", &data);
		setVSync(true);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
		
	}

	void WindowsWindow::shutdown()
	{
		SDL_DestroyWindow(window);
		--windowCount;
		if (windowCount == 0)
		{
			SDL_Quit();
		}
	}

	void WindowsWindow::onUpdate()
	{
		SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
		SDL_PumpEvents();
	}

	void WindowsWindow::setVSync(bool enabled) { data.vSync = enabled; }
	bool WindowsWindow::isVSync() const { return data.vSync; }
}