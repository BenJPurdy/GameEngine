#pragma once
#include "GameEngine_PCH.h"

#include "Core.h"
#include "LayerStack.h"



#include "Window.h"
#include "Core/Timestep.h"


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

		void pushLayer(Layer*);
		

		void pushOverlay(Layer*);
		

	private:
		void run();

		std::unique_ptr<Window> window;
		LayerStack layerStack;
		bool running = true;
		bool minimized = false;
		float lastFrameTime = 0.0f;

		static App* instance;

		friend int ::main(int argc, char** argv);

		
	};

	App* createApp();
}