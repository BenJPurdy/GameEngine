#pragma once
#include "Scene/Scene.h"
#include "Scene/Components.h"


#define SCRIPTAPI extern "C" __declspec(dllexport)
#define ENGINEAPI extern "C"


namespace GameEngine
{
	class Entity;

	namespace Scripting
	{

		struct Transform
		{
			Transform(TransformComponent t)
			{
				position = t.transform; rotation = t.rotation; scale = t.scale;
			}
			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;
		};

		Entity make(uint32_t id)
		{
			return Entity((entt::entity)id, scripting.currentScene);
		}

		//match data layout of all types
		//interface with engine -> script
		SCRIPTAPI TransformComponent scriptGetTransform(Entity e);
		SCRIPTAPI CameraComponent scriptGetCamera(Entity e);
		SCRIPTAPI Transform getTransform(uint32_t id);
		SCRIPTAPI glm::vec3 scriptGetTransformPosition(uint32_t id);

		//holds stuff to do with compiling and attaching the dll to the engine
		class Script
		{
		public:
			Scene* currentScene;
			HMODULE dllHandle = nullptr;
			bool compileScripts();
			bool loadLib();
			void unloadLib();
		};
	}

	extern Scripting::Script scripting;
}
