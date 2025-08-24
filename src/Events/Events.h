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
		MouseButtonPrsesed, MouseButtonReleased, MouseMoved, MouseScrolled,
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
		EventDispatcher(Event& event) : _event(event)
		{

		}

		template<typename T, typename F>
		bool dispatch(const F& fn)
		{
			if (_event.getEventType() == T::getStaticType())
			{
				_event.handled = fn(static_cast<T&>(_event));
				return true;
			}
			return false;
		}

	private:
		Event& _event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) { return os << e.toString(); }

	using EventCallbackFn = std::function<void(Event&)>;
}