#pragma once

#include "Core.h"

#include "../GameEngine_PCH.h"

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
	private:
		void run();

		std::unique_ptr<Window> window;
		bool running = true;
		bool minimized = false;
		float lastFrameTime = 0.0f;

		static App* instance;

		friend int ::main(int argc, char** argv);

		
	};

	App* createApp();
}