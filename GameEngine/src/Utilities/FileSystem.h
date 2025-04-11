#pragma once

#include <functional>
#include <filesystem>



namespace GameEngine
{
	enum class FileStatus
	{
		Success = 0, Invald, Locked, Other
	};
	namespace FS = std::filesystem;
	class FileSystem
	{
		
	public:
		
		static FS::path getWorkingDirectory();
		static void setWorkingDirectory(FS::path);
		static bool createDirectory(const FS::path&);
		static bool createDirectory(const std::string&);
		static bool exists(const FS::path&);
		static bool exists(const std::string&);
		static bool deleteFile(const FS::path&);
		static bool moveFile(const FS::path&, const FS::path&);
		static bool copyFile(const FS::path&, const FS::path&);
		static bool isDirectory(const FS::path&);

		static FileStatus tryOpenFile(const FS::path&);

		static bool isNewer(const FS::path&, const FS::path&);
		
		static bool move(const FS::path&, const FS::path&);
		static bool copy(const FS::path&, const FS::path&);
		static bool rename(const FS::path&, const FS::path&);
		static bool renameFilename(const FS::path&, const std::string);

		static FS::path getUnqiueFileName(const FS::path&);
		static std::string removeExtention(const std::string&);
	};


}
