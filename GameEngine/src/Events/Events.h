#pragma once

#include "GameEngine_PCH.h"
#include "Core/Core.h"

#define BIT(x) (1 << x)

namespace GameEngine
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MousebuttonPrsesed, MousebuttonReleased, MouseMoved, mouseScrolled,
		GamepadConnected, GamepadDisconnected, GamepadButtonPressed, GamepadButtonReleased,
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApp = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMousebutton = BIT(4),
		EventCategoryGamepad = BIT(5),
	};

	class Event
	{
	public:
		bool handled = false;

		virtual EventType getEventType() const = 0;
		virtual const char* getName() const = 0;
		virtual int getCategoryFlags() const = 0;
		virtual std::string toString() const { return getName(); }
		inline bool isInCategory(EventCategory c) { return getCategoryFlags() & c; }

	protected:
		bool _handled = false;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event) : event(event)
		{

		}

		template<typename t, typename F>
		bool dispatch(const F& fn)
		{
			if (event.getEventType() == T::GetStaticType())
			{
				event.handled = func(static_cast<T&>(event));
				return true;
			}
			return false;
		}

	private:
		Event& event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) { return os << e.toString(); }

	using EventCallbackFn = std::function<void(Event&)>;
}