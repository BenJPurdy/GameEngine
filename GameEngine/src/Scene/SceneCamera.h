#pragma once
#include "Renderer/Camera/Camera.h"

namespace GameEngine
{
	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void setOrthographic(float, float, float);
		void setPerspective(float, float, float);
		void setViewportSize(uint32_t, uint32_t);

		float getOrthgraphicSize() const { return orthgraphicSize; }
		void setOrthgraphicSize(float s) { orthgraphicSize = s; recalculateProjection(); }
		float getOrthgraphicNear() const { return orthgraphicNear; }
		void setOrthgraphicNear(float n) { orthgraphicNear = n; recalculateProjection(); }
		float getOrthographicFar() const { return orthographicFar; }
		void setOrthographicFar(float f) { orthographicFar = f; recalculateProjection(); }

		float getPerspectiveFOV() const { return perspectiveFOV; }
		void  setPerspectiveFOV(float s) { perspectiveFOV = s; recalculateProjection(); }
		float getPerspectiveNear() const { return perspectiveNear; }
		void  setPerspectiveNear(float n) { perspectiveNear = n; recalculateProjection(); }
		float getPerspectiveFar() const { return perspectiveFar; }
		void  setPerspectiveFar(float f) { perspectiveFar = f; recalculateProjection(); }
	
		void setProjectionType(ProjectionType t)
		{
			cameraType = t;
			recalculateProjection();
		}
	
	private:
		void recalculateProjection();

		ProjectionType cameraType = ProjectionType::Orthographic;

		float aspect = 1280.0 / 720.0f;

		float orthgraphicSize = 10.0f;
		float orthgraphicNear = -1.0f;
		float orthographicFar = 1.0f;

		float perspectiveFOV = glm::radians(60.0f);
		float perspectiveNear = 0.01f;
		float perspectiveFar = 1000.0f;
	};
}
