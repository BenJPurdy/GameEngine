#pragma once
#include "Camera.h"
#include "Core/Timestep.h"
#include "Events/Events.h"
#include "Events/MouseEvent.h"

#include <glm/glm.hpp>



namespace GameEngine
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float, float, float, float);

		void onUpdate(Timestep);
		void onEvent(Event&);

		inline float getDistance() const { return distance; }
		inline void setDistance(float d) { distance = d; }

		inline void setViewportSize(float w, float h) { width = w; height = h; }
		glm::mat4 getViewProjection() const { return projection * viewMatrix; }

		glm::vec3 getUpdirection() const;
		glm::vec3 getRightDirection() const;
		glm::vec3 getForwardDirection() const;
		const glm::vec3& getPosition() const { return position; }
		glm::quat getOrientation() const;

		float getPitch() const { return pitch; }
		float getYaw() const { return yaw; }

	private:
		void updateProjection();
		void updateView();

		bool onMouseScroll(MouseScrolledEvent&);

		void mousePan(const glm::vec2&);
		void mouseRotate(const glm::vec2&);
		void mouseZoom(float);

		glm::vec3 calculatePosition() const;
		std::pair<float, float> panSpeed() const;
		float rotationSpeed() const;
		float zoomSpeed() const;

		float fov = 45.0f, aspect = 1280.0f / 720.0f, nearClip = 0.1f, farClip = 1000.0f;

		glm::mat4 viewMatrix;
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 focalPoint = { 0.0f, 0.0f, 0.0f };

		glm::vec2 initialMousePosition = { 0.0f, 0.0f };

		float distance = 10.0f;
		float pitch = 0.0f, yaw = 0.0f;

		float width = 1280, height = 720;

	};
}
