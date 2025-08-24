#pragma once
#include "Events.h"

namespace GameEngine
{
	class KeyEvent : public Event
	{
	public: 
		inline int getKeyCode() const { return keyCode; }
		int getCategoryFlags() const override { return EventCategoryApp | EventCategoryInput; }
	protected:
		KeyEvent(int kc) : keyCode(kc){}
		int keyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int kc, int kr) : KeyEvent(kc), repeatCount(kr) {};

		inline int getRepeatCount() const { return repeatCount; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KEY_PRESSED_EVENT: " << keyCode << "Repetition: " << repeatCount;
			return ss.str();
		}

		static EventType getStaticType() { return EventType::KeyPressed; }
		EventType getEventType() const override { return getStaticType(); }

		const char* getName() const override { return "KeyPressed"; }
	private:
		int repeatCount;
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int k) : KeyEvent(k) {}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KEY_TYPED_EVENT: " << keyCode;
			return ss.str();
		}

		static EventType getStaticType() { return EventType::KeyTyped; }
		EventType getEventType() const override { return getStaticType(); }
		const char* getName() const override { return "KeyTyped"; }
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int k) : KeyEvent(k) {}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KEY_RELEASE_EVENT: " << keyCode;
			return ss.str();
		}

		static EventType getStaticType() { return EventType::KeyReleased; }
		EventType getEventType() const override { return getStaticType(); }
		const char* getName() const override { return "KeyReleased"; }
	};
}