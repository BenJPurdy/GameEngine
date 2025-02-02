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

		}
	}
}