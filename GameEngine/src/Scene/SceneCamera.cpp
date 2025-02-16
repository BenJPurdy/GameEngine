#include "GameEngine_PCH.h"
#include "SceneCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace GameEngine
{
	SceneCamera::SceneCamera()
	{
		recalculateProjection();
	}

	void SceneCamera::setOrthographic(float s, float n, float f)
	{
		cameraType = ProjectionType::Orthographic;
		orthgraphicSize = s;
		orthgraphicNear = n;
		orthographicFar = f;
	}

	void SceneCamera::setPerspective(float v, float n, float f)
	{
		cameraType = ProjectionType::Perspective;
		perspectiveFOV = v;
		perspectiveFar = f;
		perspectiveNear = n;

		recalculateProjection();
	}

	void SceneCamera::setViewportSize(uint32_t w, uint32_t h)
	{
		if (w > 0 && h > 0)
		{
			aspect = (float)w / (float)h;
			recalculateProjection();
		}
		else { LOG_ERROR("SceneCameraSetViewportSize: Setting viewport to width {0}, height {1}", w, h); }
	}

	void SceneCamera::recalculateProjection()
	{
		if (cameraType == ProjectionType::Orthographic)
		{
			float l = -orthgraphicSize * 0.5 * aspect;
			float r = orthgraphicSize * 0.5 * aspect;
			float t = orthgraphicSize * 0.5;
			float b = -orthgraphicSize * 0.5;

			m_Projection = glm::ortho(l, r, b, t, orthgraphicNear, orthographicFar);
		}

		else if (cameraType == ProjectionType::Perspective)
		{
			m_Projection = glm::perspective(perspectiveFOV, aspect, perspectiveNear, perspectiveFar);
		}
	}
}