#include "GameEngine_PCH.h"
#include "PlatformUtils.h"

#include "Utilities/Windows/WindowsPlatformUtils.h"


namespace GameEngine
{
	std::string FileDialogs::openFile(const char* filter)
	{
		return WindowsFileDialogs::openFile(filter);
	}

	std::string FileDialogs::saveFile(const char* filter)
	{
		return WindowsFileDialogs::saveFile(filter);
	}

}