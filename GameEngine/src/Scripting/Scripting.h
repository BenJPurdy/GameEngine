#pragma once
#include "Scene/Components.h"
#include "Scene/Entity.h"

#define SCRIPTAPI extern "C"
#define ENGINEAPI extern "C"

namespace GameEngine
{
	namespace Scripting
	{
		//match data layout of all types
		//interface with engine -> script
		SCRIPTAPI TransformComponent scriptGetTransform(Entity e) { return e.getComponent<TransformComponent>(); }
		SCRIPTAPI CameraComponent scriptGetCamera(Entity e) { return e.getComponent<CameraComponent>(); }
		

		//holds stuff to do with compiling and attaching the dll to the engine
		class Script
		{

		};
	}
}
