#pragma once
#include <chrono>


namespace GameEngine
{

	struct WindowProps
	{
		std::string title;
		uint32_t w, h;

		WindowProps(const std::string& title = "HSIAGE",
			uint32_t w = 1280, uint32_t h = 720)
			: title(title), w(w), h(h) {}
	};

	class Window
	{
	public:
		virtual ~Window() {}
		virtual void onUpdate() = 0;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		//window attributes
		virtual void setVSync(bool) = 0;
		virtual bool isVSync() const = 0;

		virtual void* getNativeWindow() const = 0;

		static std::unique_ptr<Window> create(const WindowProps& props = WindowProps());



	};

}
