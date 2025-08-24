#pragma once
#include "Utilities/PlatformUtils.h"

namespace GameEngine
{
	class WindowsFileDialogs : FileDialogs
	{
	public:
		static std::string openFile(const char*);
		static std::string saveFile(const char*);
	};
}
