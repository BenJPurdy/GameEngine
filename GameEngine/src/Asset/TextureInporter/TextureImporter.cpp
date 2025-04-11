#include "GameEngine_PCH.h"
#include "TextureImporter.h"
#include "stb/stb_image.h"
#include "Utilities/FileSystem.h"
#include <iostream>

namespace GameEngine
{
	DataBuffer TextureImporter::toBufferFromFile(const std::filesystem::path& p, uint32_t& oW, uint32_t& oH)
	{
		//FileStatus fileStatus = FileSystem::tryOpenFile(p);
		//DataBuffer buff;
		//std::string pathStr = p.string();
		//
		//if (stbi_is_hdr(pathStr.c_str()))
		//{
		//	//buff.
		//}
		//
		return DataBuffer();
	}
}