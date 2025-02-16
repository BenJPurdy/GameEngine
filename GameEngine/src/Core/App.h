#pragma once
#include "GameEngine_PCH.h"

#include "Core.h"
#include "LayerStack.h"
#include "ImGui/ImGuiLayer.h"



#include "Window.h"
#include "Core/Timestep.h"

#include "Events/Events.h"
#include "Events/ApplicationEvents.h"


int main(int argc, char** argv);

namespace GameEngine
{
	class App
	{
	public:
		App(const std::string& name = "HSIAGE");
		~App();

		inline Window& getWindow() { return *window; }
		void close();
		static App& get() { return *instance; }

		void onEvent(Event&);
		void pushLayer(Layer*);
		void pushOverlay(Layer*);

		ImGuiLayer* getImguiLayer() { return imguiLayer; }
		

	private:
		void run();
		bool onWindowClose(WindowCloseEvent&);
		bool onWindowResize(WindowResizeEvent&);


		std::unique_ptr<Window> window;
		LayerStack layerStack;
		ImGuiLayer* imguiLayer;
		bool running = true;
		bool minimized = false;
		float lastFrameTime = 0.0f;

		static App* instance;

		friend int ::main(int argc, char** argv);

		
	};

	App* createApp();
}