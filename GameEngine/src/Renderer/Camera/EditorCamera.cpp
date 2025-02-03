#include "GameEngine_PCH.h"
#include "EditorCamera.h"
#include "Core/Input.h"
#include "Core/KeyCodes.h"
#include "Core/MouseCodes.h"

#include "GLFW/glfw3.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace GameEngine
{
	EditorCamera::EditorCamera(float fov, float ar, float nC, float fC) : 
		fov(fov), aspect(ar), nearClip(nC), farClip(fC)  { updateView(); }

	void EditorCamera::onUpdate(Timestep t)
	{
		if (Input::isKeyPressed(Key::LeftAlt))
		{
			const glm::vec2& mouse{Input::getMouseX(), Input::getMouseY()};
			//what is the magic numebr doing???
			glm::vec2 delta = (mouse - initialMousePosition) * 0.003f;
			initialMousePosition = mouse;

			if (Input::isMouseButtonPressed(Mouse::ButtonMiddle)) mousePan(delta);
			else if (Input::isMouseButtonPressed(Mouse::ButtonLeft)) mouseRotate(delta);
			else if (Input::isMouseButtonPressed(Mouse::ButtonRight)) mouseZoom(delta.y);
		}
		updateView();
	}

	void EditorCamera::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT_FN(EditorCamera::onMouseScroll));
	}

	glm::vec3 EditorCamera::getUpdirection() const
	{
		return glm::rotate(getOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::getRightDirection() const
	{
		return glm::rotate(getOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::getForwardDirection() const
	{
		return glm::rotate(getOrientation(), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	glm::quat EditorCamera::getOrientation() const
	{
		return glm::quat(glm::vec3(-pitch, -yaw, 0.0f));
	}

	void EditorCamera::updateView()
	{
		position = calculatePosition();

		glm::quat o = getOrientation();
		viewMatrix = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(o);
		// remove this later to do it in the shader maybe? seems expensive to do
		// on the cpu every frame
		// then again, less math is good math
		viewMatrix = glm::inverse(viewMatrix);
	}

	bool EditorCamera::onMouseScroll(MouseScrolledEvent& e)
	{
		//RENAME e.getY to getOffsetY
		float d = e.getY() * 0.1f;
		mouseZoom(d);
		updateView();
		return false;
	}

	void EditorCamera::mousePan(const glm::vec2& d)
	{
		auto [xSpeed, ySpeed] = panSpeed();
		focalPoint += -getRightDirection() * d.x * xSpeed * distance;
		focalPoint += getUpdirection() * d.y * ySpeed * distance;
	}

	void EditorCamera::mouseRotate(const glm::vec2& d)
	{
		float yawSign = getUpdirection().y < 0 ? -1.0f : 1.0f;
		yaw += yawSign * d.x * rotationSpeed();
		pitch += d.y * rotationSpeed();
	}

	void EditorCamera::mouseZoom(float d)
	{
		distance -= d * zoomSpeed();
		if (distance < 1.0f)
		{
			focalPoint += getForwardDirection();
			distance = 1.0f;
		}
	}

	glm::vec3 EditorCamera::calculatePosition() const
	{
		return focalPoint - getForwardDirection() * distance;
	}

	std::pair<float, float> EditorCamera::panSpeed() const
	{
		//WHY ALL THESE NUMBERS
		float x = std::min(width / 1000.0f, 2.4f);
		float xF = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(height / 1000.0f, 2.4f);
		float yF = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		return {xF, yF};
	}

	float EditorCamera::rotationSpeed() const { return 0.8f; }

	float EditorCamera::zoomSpeed() const
	{
		float d = distance * 0.2f;
		d = std::max(distance, 0.0f);
		float speed = d * d;
		speed = std::min(speed, 100.0f);
		return speed;
	}

}