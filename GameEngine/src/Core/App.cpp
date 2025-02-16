#include "GameEngine_PCH.h"
#include "App.h"
#include "Input.h"
#include "GLFW/glfw3.h"


namespace GameEngine
{

#define BIND_EVENT_FUNC(x) std::bind(&App::x, this, std::placeholders::_1)

	App* App::instance = nullptr;

	App::App(const std::string& name)
	{
		instance = this;
		window = Window::create(WindowProps(name));
		window->setEventCallback(std::bind(&App::onEvent, this, std::placeholders::_1));

		Input::init();
		Input::setCallback(BIND_EVENT_FUNC(App::onEvent));

		imguiLayer = new ImGuiLayer();
		pushLayer(imguiLayer);
	}

	App::~App() {}

	void App::close() { running = !running; }

	void App::run()
	{
		while (running)
		{
			auto time = (float)glfwGetTime();
			Timestep timestep = time - lastFrameTime;
			lastFrameTime = time;
			if (!minimized)
			{
				for (auto* l : layerStack)
				{
					l->onUpdate(timestep);
				}

				imguiLayer->begin();
				{
					for (auto* l : layerStack)
					{
						l->onImGuiRender();
					}
				}
				imguiLayer->end();

				Input::update();

				
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
	
	void App::onEvent(Event& e)
	{
		EventDispatcher d(e);
		d.dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(onWindowClose));
		d.dispatch<WindowResizeEvent>(BIND_EVENT_FUNC(onWindowResize));

		for (auto it = layerStack.rbegin(); it != layerStack.rend(); ++it)
		{
			if (e.handled) break;
			(*it)->onEvent(e);
		}
	}

	bool App::onWindowClose(WindowCloseEvent& e)
	{
		running = false;
		return true;
	}

	bool App::onWindowResize(WindowResizeEvent& e)
	{
		if (e.getWidth() == 0 || e.getHeight() == 0)
		{
			minimized = true;
			return false;
		}
		minimized = false;
		return false;
	}
		


}


