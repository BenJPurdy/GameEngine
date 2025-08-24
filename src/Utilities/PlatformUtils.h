#pragma once
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

namespace GameEngine
{
	class FileDialogs
	{
	public:
		static std::string openFile(const char*);
		static std::string saveFile(const char*);
	};
}
