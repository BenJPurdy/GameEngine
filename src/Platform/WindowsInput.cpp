#include "GameEngine_PCH.h"
#include "Core/Input.h"
#include <GLFW/glfw3.h>
#include "Core/App.h"

namespace GameEngine
{
	static EventCallbackFn callback;

	void Input::init()
	{

	}

	void Input::setCallback(const EventCallbackFn& c) { callback = c; }
	
	void Input::update()
	{

	}

	bool Input::isKeyPressed(KeyCode k)
	{
		auto window = static_cast<GLFWwindow*>(App::get().getWindow().getNativeWindow());
		auto state = glfwGetKey(window, static_cast<int32_t>(k));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::isMouseButtonPressed(MouseCode m)
	{
		auto window = static_cast<GLFWwindow*>(App::get().getWindow().getNativeWindow());
		auto state = glfwGetMouseButton(window, m);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> Input::getMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(App::get().getWindow().getNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return { (float)x, (float)y };
	}

	float Input::getMouseX()
	{
		auto [x, y] = getMousePosition();
		return x;
	}

	float Input::getMouseY()
	{
		auto [x, y] = getMousePosition();
		return y;
	}


}