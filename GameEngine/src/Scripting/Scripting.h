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

		void populateEntityPointers(ScriptComponent&);
		void populatePointers(HMODULE&, ScriptComponent&);
		void freeDll(HMODULE&);
		FARPROC getFunc(HMODULE, std::string);
		bool loadLib(HMODULE&);

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
			Ref<Scene> currentScene;
			HMODULE dllHandle = nullptr;
			bool compileScripts();
			bool loadLib();
			FARPROC getFn(std::string);
			void unloadLib();
		};
	}

	
}
