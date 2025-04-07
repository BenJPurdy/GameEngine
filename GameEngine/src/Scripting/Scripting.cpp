#include "GameEngine_PCH.h"
#include "Scripting.h"
#include "Utilities/FileManagement.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"
#include "Core/Input.h"
#include "API/API.h"





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
		
	// currently non-functional, should not be called
	void populateEntityPointers(ScriptComponent& s)
	{
		//s.onStartPtr = getFunc(scripting.dllHandle, s.onStart);
		//s.onUpdatePtr = getFunc(scripting.dllHandle, s.onUpdate);
		//s.onCollisionEnterPtr = getFunc(scripting.dllHandle, s.onCollisionEnter);
		//s.onCollisionExitPtr = getFunc(scripting.dllHandle, s.onCollisionExit);
		//s.onDestoryPtr = getFunc(scripting.dllHandle, s.onDestory);
	}

	void populatePointers(HMODULE& dll, ScriptComponent& s)
	{
		s.onStartPtr =			getFunc(dll, std::string(s.script + "_onStart"));
		s.onUpdatePtr =			getFunc(dll, std::string(s.script + "_onUpdate"));
		s.onCollisionEnterPtr = getFunc(dll, std::string(s.script + "_onCollisionEnter"));
		s.onCollisionExitPtr =	getFunc(dll, std::string(s.script + "_onCollisionExit"));
		s.onDestoryPtr =		getFunc(dll, std::string(s.script + "_onDestroy"));
	}
	

	
	
	bool Script::compileScripts()
	{
		std::cout << "\x1b[44m \x1b[37m START COMPILING SCRIPTS  \x1b[0m\n";
		std::vector<std::string> objectFiles;
		std::vector<std::string> recompileFiles;
		std::string objectFolder = "./objects";
		//building scripts into objects
		std::string scriptFolder = "./assets/Scripts";
		namespace FM = FileManagment;
		std::vector<std::pair<std::string, std::string>> files;
		//get our script files
		FM::getFilesInFolder(&files, scriptFolder);

		scriptFolder = "./assets/Scripts/Internal";
		//get our internal script files
		FM::getFilesInFolder(&files, scriptFolder);
		std::filesystem::path dllPath = FM::getFile("./JFAaB.dll", "./");
		std::filesystem::file_time_type dllTime;
		bool isModified = false;
		
		if (!dllPath.empty())
		{
			dllTime = std::filesystem::last_write_time(dllPath);
		}
		else
		{
			LOG_WARN("No DLL found, compiling all scripts");
			isModified = true;
		}
		
		for (auto& f : files)
		{
			if (f.second == ".cpp")
			{
				size_t nameAt = f.first.find("\\");
				std::string name = f.first.substr(nameAt + 1);
				name = name.substr(0, name.find("."));
				auto fileTime = std::filesystem::last_write_time(f.first);
				LOG_TRACE("checking path: {0}. diff (src - dll) {1}", f.first, fileTime.time_since_epoch().count() - dllTime.time_since_epoch().count());
				if (fileTime > dllTime || isModified)
				{
					std::string cmd = "g++ -c ";
#ifdef DEBUG
					cmd += "-g ";
#endif
					cmd += f.first;
					cmd += " -std=c++17 -O3 -I./3rdParty -lmingw32";
					std::string output = " -o objects/";
					output += name;
					output += ".o";
					cmd += output;
					LOG_TRACE("Compiling {0} with {1}", f.first, cmd);
					system(cmd.c_str());
				}
			}
		}
		
		FM::getFilesInFolder(&objectFiles, objectFolder);

		LOG_TRACE("Compiling dll");
		if (objectFiles.size() > 0)
		{
			std::string cmd = "g++ ";
			for (auto& i : objectFiles)
			{
				cmd += i;
				cmd += " ";
			}

			//cmd += "-I ";

			//optimising this could use -c to make object files of each script
			// the link them as a seperate compile step to produce the dll, allows
			// for only compiling modified files again and then just linking all
			// step one: -c with each modified script
			// step two: -shared with object files

			cmd += "-shared";
#ifdef DEBUG
			cmd += " -g";
#endif
			cmd += " -o JFAaB.dll";

			
			//cmd = g++ [files] -shared -o JFAaB.dll 
			//system(cmd);

			LOG_WARN(cmd);

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
		FARPROC addr = GetProcAddress(handle, name.c_str());
		return addr;
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

		void* testFn = GetProcAddress(m, "MyEntity::test");
		if (testFn == nullptr)
		{
			LOG_WARN("Failed to get name from class");
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


