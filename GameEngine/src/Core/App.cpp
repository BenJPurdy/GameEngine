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

			for (auto* l : layerStack)
			{
				l->onUpdate(timestep);
			}

			window->onUpdate();
		}
	}

	void App::pushLayer(Layer* l)
	{
		layerStack.push(l);
		l->onAttach();
	}

	void App::pushOverlay(Layer* l)
	{
		layerStack.pushOverlay(l);
		l->onAttach();
	}
	


}


