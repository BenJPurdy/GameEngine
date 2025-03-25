#include "GameEngine_PCH.h"
#include "Scripting.h"
#include "Utilities/FileManagement.h"
#include "Scene/Entity.h"

typedef void (*function)();

namespace GameEngine
{
	Scripting::Script scripting;
namespace Scripting
{

	SCRIPTAPI TransformComponent scriptGetTransform(Entity e) { return e.getComponent<TransformComponent>(); }
	SCRIPTAPI CameraComponent scriptGetCamera(Entity e) { return e.getComponent<CameraComponent>(); }
	SCRIPTAPI Transform getTransform(uint32_t id)
	{
		TransformComponent TRC = make(id).getComponent<TransformComponent>();
		Transform t(TRC);
		return t;
	}

	SCRIPTAPI void scriptSetTransform(Entity e, Transform t)
	{
		auto& tr = e.getComponent<TransformComponent>();
		tr.transform = t.position;
		tr.rotation= t.rotation;
		tr.scale = t.scale;
		tr.mod = true;


	}
	SCRIPTAPI glm::vec3 scriptGetTransformPosition(uint32_t id) 
	{
		return make(id).getComponent<TransformComponent>().transform;
	}

	SCRIPTAPI void sayHello() { LOG_TRACE("Hello from the script from the engine"); }
	
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

			cmd += "-I ";

#ifdef DEBUG
			cmd += "-shared -O3 -g -o JFAaB.dll";
#else
			cmd += "-shared -O3 -o JFAaB.dll";
#endif
			//cmd = g++ [files] -shared -o JFAaB.dll 
			//system(cmd);

			if (system(cmd.c_str()) != 0) return false;
			return true;
		}

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
		function f = (function)fn;
		f();
		return true;
	}

	void Script::unloadLib()
	{
		FreeLibrary(dllHandle);
	}
}
}


