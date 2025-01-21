#pragma once

#include "Core.h"
#include "Window.h"

int main(int argc, char** argv);

namespace GameEngine
{
	class Application
	{
	public:
		Application(const std::string& name = "HSIAGE");
		~Application();

		inline Window& getWindow() { return *window; }
		void Close();
		static Application& Get() { return *instance; }
	private:
		Run();

		std::unique_ptr<Window> window;
		bool running = true;
		bool minimized = false;
		float lastFrameTime = 0.0f;

		static Application instance;

		friend int ::main(int argc, char** argv);

		Application* CreteApplication();
	};
}