#include "GameEngine_PCH.h"
#include "App.h"
#include "SDL.h"


namespace GameEngine
{

#define BIND_EVENT_FUNC(x) std::bind(&App::x, this, std::placeholders::_1)

	App* App::instance = nullptr;

	App::App(const std::string& name)
	{
		instance = this;
		window = Window::create(WindowProps(name));
	}

	App::~App() {}

	void App::close() { running = !running; }

	void App::run()
	{
		while (running)
		{
			auto time = SDL_GetTicks();
			Timestep timestep = time - lastFrameTime;
			lastFrameTime = time;

			window->onUpdate();
		}
	}


}


