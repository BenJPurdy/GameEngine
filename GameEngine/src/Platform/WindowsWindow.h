#pragma once

#include "Core/Window.h"
#include "SDL.h"

namespace GameEngine
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps&);

		virtual ~WindowsWindow();

		void onUpdate() override;

		uint32_t getWidth() const override { return data.width; }
		uint32_t getHeight() const override { return data.height; }

		void setVSync(bool) override;
		bool isVSync() const override;

		void* getNativeWindow() const override { return window; }

	private:
		void init(const WindowProps&);
		void shutdown();


		SDL_Window* window;
		struct WindowData
		{
			std::string title;
			uint32_t width, height;
			bool vSync;
		};

		WindowData data;

	};
}
