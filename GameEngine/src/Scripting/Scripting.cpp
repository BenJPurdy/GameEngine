#include "GameEngine_PCH.h"
#include "Scripting.h"
#include "Utilities/FileManagement.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"





namespace GameEngine
{
	
namespace Scripting
{
	Scripting::Script scripting;
	//SCRIPTAPI Transform getTransform(uint32_t id)
	//{
	//	TransformComponent TRC = make(id).getComponent<TransformComponent>();
	//	Transform t(TRC);
	//	return t;
	//}

	//test function
	SCRIPTAPI void scriptSayHello() { LOG_TRACE("Hello from the script from the engine"); }

	SCRIPTAPI void speak() {}
	SCRIPTAPI Transform scriptGetTransform(Entity e)
	{
		return Transform(e.getComponent<TransformComponent>());
	}
	SCRIPTAPI void scriptSetTransform(Entity e, Transform t)
	{
		auto& tr = e.getComponent<TransformComponent>();
		tr.transform = t.position;
		tr.rotation= t.rotation;
		tr.scale = t.scale;
		tr.mod = true;
	}
	SCRIPTAPI void scriptOnCollisionEnter(Entity e)
	{

	}
	SCRIPTAPI void scriptOnCollisionExit(Entity e)
	{

	}
	SCRIPTAPI void scriptOnStart(Entity e)
	{

	}
	SCRIPTAPI void scriptOnDestroy(Entity e)
	{

	}
	SCRIPTAPI void scriptOnUpdate(Entity e, Timestep ts)
	{

	}
	SCRIPTAPI void playSound(int index)
	{
		LOG_TRACE("Scene is editor ({0})", (int)scripting.currentScene->isEditorScene);
		scripting.currentScene->getAudio()->playSound(0);
	}

	void populateEntityPointers(ScriptComponent& s)
	{
		s.onStartPtr = getFunc(scripting.dllHandle, s.onStart);
		s.onUpdatePtr = getFunc(scripting.dllHandle, s.onUpdate);
		s.onCollisionEnterPtr = getFunc(scripting.dllHandle, s.onCollisionEnter);
		s.onCollisionExitPtr = getFunc(scripting.dllHandle, s.onCollisionExit);
		s.onDestoryPtr = getFunc(scripting.dllHandle, s.onDestory);
	}

	void populatePointers(HMODULE& dll, ScriptComponent& s)
	{
		if (s.onStart.size())			s.onStartPtr =			getFunc(dll, s.onStart);
		if (s.onUpdate.size())			s.onUpdatePtr =			getFunc(dll, s.onUpdate);
		if (s.onCollisionEnter.size())	s.onCollisionEnterPtr = getFunc(dll, s.onCollisionEnter);
		if (s.onCollisionExit.size())	s.onCollisionExitPtr =	getFunc(dll, s.onCollisionExit);
		if (s.onDestory.size())			s.onDestoryPtr =		getFunc(dll, s.onDestory);
	}
	

	
	
	bool Script::compileScripts()
	{
		std::cout << "\x1b[44m \x1b[37m START COMPILING SCRIPTS  \x1b[0m\n";
		std::string scriptFolder = "./assets/Scripts";
		namespace FM = FileManagment;
		std::vector<std::pair<std::string, std::string>> files;
		FM::getFilesInFolder(&files, scriptFolder);
		std::vector<std::string> includeFiles;
		for (auto& f : files)
		{
			if (f.second == ".cpp")
			{
				includeFiles.push_back(f.first);
			}
		}
		if (includeFiles.size() > 0)
		{
			std::string cmd = "g++ ";
			for (auto& i : includeFiles)
			{
				cmd += i;
				cmd += " ";
			}

			//cmd += "-I ";

#ifdef DEBUG
			cmd += "-shared -O3 -g -lmingw32 -o JFAaB.dll";
#else
			cmd += "-shared -O3 -lmingw32 -o JFAaB.dll";
#endif
			//cmd = g++ [files] -shared -o JFAaB.dll 
			//system(cmd);

			if (system(cmd.c_str()) != 0) return false;
			return true;
		}

		return true;
	}

	FARPROC getFunc(HMODULE handle, std::string name)
	{
		if (handle == nullptr)
		{
			LOG_ERROR("DLL is not loaded");
		}
		return GetProcAddress(handle, name.c_str());
	}
	void freeDll(HMODULE& m)
	{
		if (m == nullptr)
		{
			LOG_WARN("Dll was not loaded");
		}
		FreeLibrary(m);
	}

	FARPROC Script::getFn(std::string name)
	{
		if (dllHandle == nullptr)
		{
			LOG_ERROR("DLL is not loaded");
		}
		return GetProcAddress(dllHandle, name.c_str());
	}
	bool loadLib(HMODULE& m)
	{
		m = LoadLibraryA("JFAaB.dll");
		if (!m)
		{
			LOG_ERROR("Failed to load scripts DLL");
			return false;
		}
		void* fn = GetProcAddress(m, "testFunction");
		if (fn == nullptr) return false;
		voidFn f = (voidFn)fn;
		f();
		return true;
	}
	bool Script::loadLib()
	{
		dllHandle = LoadLibraryA("JFAaB.dll");
		if (!dllHandle)
		{
			LOG_ERROR("Failed to load scripts DLL");
			return false;
		}
		void* fn = GetProcAddress(dllHandle, "testFunction");
		if (fn == nullptr) return false;
		voidFn f = (voidFn)fn;
		f();
		return true;
	}

	void Script::unloadLib()
	{
		if (dllHandle == nullptr)
		{
			LOG_WARN("Dll was not loaded");
		}
		else
		{
			LOG_WARN("Dll was loaded");
		}
		FreeLibrary(dllHandle);
	}

	Entity make(uint32_t id)
	{
		return Entity((entt::entity)id, scripting.currentScene.get());
	}
}
}


