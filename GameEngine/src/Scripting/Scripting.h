#pragma once

#include "Scene/Components.h"


#define SCRIPTAPI extern "C" __declspec(dllexport)


namespace GameEngine
{
	class Entity;
	class Scene;

	namespace Scripting
	{
		class Script;
		extern Script scripting;
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

		void populateEntityPointers(ScriptComponent&);

		Entity make(uint32_t);
		//Entity make(uint32_t id)
		//{
		//	return Entity((entt::entity)id, scripting.currentScene);
		//}

		//match data layout of all types(
		//interface with engine -> script

		//holds stuff to do with compiling and attaching the dll to the engine
		class Script
		{
		public:
			Scene* currentScene;
			HMODULE dllHandle = nullptr;
			bool compileScripts();
			bool loadLib();
			FARPROC getFn(std::string);
			void unloadLib();
		};
	}

	
}
