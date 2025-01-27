#pragma once

#include "Events.h"

namespace GameEngine
{
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y) : mouseX(x), mouseY(y) {}

		inline float getX() const { return mouseX; }
		inline float getY() const { return mouseY; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << mouseX << ", " << mouseY;
			return ss.str();
		}

		static EventType getStaticType() { return EventType::MouseMoved; }
		EventType getEventType() const override{ return getStaticType(); }
		const char* getName() const override { return "MouseMoved"; }
		int getCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; }

	private:
		float mouseX, mouseY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float x, float y) : offsetX(x), offsetY(y) {}

		inline float getX() const { return offsetX; }
		inline float getY() const { return offsetY; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << offsetX << ", " << offsetY;
			return ss.str();
		}

		static EventType getStaticType() { return EventType::MouseScrolled; }
		EventType getEventType() const override { return getStaticType(); }
		const char* getName() const override { return "MouseMoved"; }
		int getCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; }

	private:
		float offsetX, offsetY;
	};

	class MouseButtonEvent : public Event
	{
	public:
		inline int getMouseButton() const { return button; }
		int getCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; }

	protected:
		MouseButtonEvent(int button) : button(button) {}
		int button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int b) : MouseButtonEvent(b) {}

	std::string toString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << button;
		return ss.str();
	}

	static EventType getStaticType() { return EventType::MouseButtonPrsesed; }
	EventType getEventType() const override { return getStaticType(); }
	const char* getName() const override { return "MouseButtonPressed"; }
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{

	public:
		MouseButtonReleasedEvent(int b) : MouseButtonEvent(b) {}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << button;
			return ss.str();
		}

		static EventType getStaticType() { return EventType::MouseButtonReleased; }
		EventType getEventType() const override { return getStaticType(); }
		const char* getName() const override { return "MouseButtonReleased"; }
	};
}


