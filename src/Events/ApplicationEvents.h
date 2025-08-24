#pragma once


#include "Events.h"

namespace GameEngine
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32_t w, uint32_t h) : w(w), h(h) {}

		inline uint32_t getWidth() const { return w; }
		inline uint32_t getHeight() const { return h; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << w << ", " << h;
			return ss.str();
		}

		static EventType getStaticType() { return EventType::WindowResize; }
		EventType getEventType() const override { return getStaticType(); }
		const char* getName() const override { return "WindowResize"; }
		int getCategoryFlags() const override { return EventCategoryApp; }

	private:
		uint32_t w, h;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}
		static EventType getStaticType() { return EventType::WindowClose; }
		EventType getEventType() const override { return getStaticType(); }
		const char* getName() const override { return "WindowClose"; }
		int getCategoryFlags() const override { return EventCategoryApp; }
	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() {};
		static EventType getStaticType() { return EventType::AppTick; }
		EventType getEventType() const override { return getStaticType(); }
		const char* getName() const override { return "AppTick"; }
		int getCategoryFlags() const override { return EventCategoryApp; }
	};

	class AppUpdateEvent : public Event
	{
		AppUpdateEvent() {};
		static EventType getStaticType() { return EventType::AppUpdate; }
		EventType getEventType() const override { return getStaticType(); }
		const char* getName() const override { return "AppUpdate"; }
		int getCategoryFlags() const override { return EventCategoryApp; }
	};

	class AppRenderEvent : public Event
	{
		AppRenderEvent() {};
		static EventType getStaticType() { return EventType::AppRender; }
		EventType getEventType() const override { return getStaticType(); }
		const char* getName() const override { return "AppRender"; }
		int getCategoryFlags() const override { return EventCategoryApp; }
	};
}