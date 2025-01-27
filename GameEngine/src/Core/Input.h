#pragma once

#include "Core.h"

#include "KeyCodes.h"
#include "MouseCodes.h"
#include "Events/Events.h"
#include <glm/glm.hpp>

namespace GameEngine
{
	class Input
	{
	public:
		static void init();
		static void setCallback(const EventCallbackFn&);
		static void update();
		static bool isKeyPressed(KeyCode);
		static bool isMouseButtonPressed(MouseCode);
		static std::pair<float, float> getMousePosition();
		static float getMouseX();
		static float getMouseY();

	};
}