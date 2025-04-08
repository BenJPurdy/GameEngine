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
			//we don't have the dll, we should compile everything, just to be safe
			LOG_WARN("No DLL found, compiling all scripts");
			isModified = true;
		}
		
		for (auto& f : files)
		{
			if (f.second == ".cpp")
			{
				//substr into the filename to extract the name of the file without the prefix folders or suffix filetype
				//(via a quirk of std::filesystem populating the list with "\" when it adds the file name)
				//probably not portable but it works
				size_t nameAt = f.first.find("\\");
				std::string name = f.first.substr(nameAt + 1);
				name = name.substr(0, name.find("."));
				//do we need to update the file
				auto fileTime = std::filesystem::last_write_time(f.first);
				LOG_TRACE("checking path: {0}. diff (src - dll) {1}", f.first, 
					fileTime.time_since_epoch().count() - dllTime.time_since_epoch().count());
				//if the file was modified *AFTER* the DLL was created
				//we need to recompile the file
				//or we're doing everything, in which case,
				//compile the file to a .o object file
				if (fileTime > dllTime || isModified)
				{
					std::string cmd = "g++ -c ";
#ifdef DEBUG		// if we're debugging we may as well add debug symbols
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
		//get all our .o files from the objects folder
		FM::getFilesInFolder(&objectFiles, objectFolder);

		//link the object files into a DLL
		LOG_TRACE("Linking dll");
		if (objectFiles.size() > 0)
		{
			std::string cmd = "g++ ";
			for (auto& i : objectFiles)
			{
				cmd += i;
				cmd += " ";
			}

			cmd += "-shared";
#ifdef DEBUG // if we're debugging, lets make sure the debug symbols exist
			//luckly if this isn't doing anything the compiler will discard it
			cmd += " -g";
#endif
			cmd += " -o JFAaB.dll";

			
			//cmd = g++ [files] -shared -o JFAaB.dll 
			//system(cmd);

			//lets print the command just to make sure it's doing everythign we expect
			LOG_WARN(cmd);

			//return 0 means good, anything else is a failstate
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
		//this is unsafe but will return nullptr if it's an invalid
		//address, so we can make our checks at at the caller
		FARPROC addr = GetProcAddress(handle, name.c_str());
		return addr;
	}
	void freeDll(HMODULE& m)
	{
		if (m == nullptr)
		{
			LOG_WARN("Dll was not loaded");
		}
		//this has no effect if called on a nullptr,
		//but we'll warnthe user anyway
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
		//lets run our test function to make sure it's built the
		//dll. 
		void* fn = GetProcAddress(m, "testFunction");
		if (fn == nullptr) return false;
		voidFn f = (voidFn)fn;
		f();
		//testing for class name coherency, apparently not doable due to shifting between
		//gcc and msvc compilers between engine and script build systems
		void* testFn = GetProcAddress(m, "MyEntity::test");
		if (testFn == nullptr)
		{
			//its not a fatal error so we can just inform the user
			//we didn't find the function
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


