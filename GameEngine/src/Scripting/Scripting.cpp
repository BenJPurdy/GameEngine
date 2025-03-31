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

	void populateEntityPointers(ScriptComponent& s)
	{
		s.onStartPtr = scripting.getFn(s.onStart);
		s.onUpdatePtr = scripting.getFn(s.onUpdate);
		s.onCollisionEnterPtr = scripting.getFn(s.onCollisionEnter);
		s.onCollisionExitPtr = scripting.getFn(s.onCollisionExit);
		s.onDestoryPtr = scripting.getFn(s.onDestory);
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

	FARPROC Script::getFn(std::string name)
	{
		return GetProcAddress(dllHandle, name.c_str());
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
		FreeLibrary(dllHandle);
	}

	Entity make(uint32_t id)
	{
		return Entity((entt::entity)id, scripting.currentScene);
	}
}
}


