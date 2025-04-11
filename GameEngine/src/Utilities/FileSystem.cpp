#include "GameEngine_PCH.h"
#include "FileSystem.h"


namespace GameEngine
{
	FS::path		FileSystem::getWorkingDirectory()
	{
		return FS::current_path();
	}
	void			FileSystem::setWorkingDirectory(FS::path p)
	{
		FS::current_path(p);
	}
	bool			FileSystem::createDirectory(const FS::path& dir)
	{
		return FS::create_directories(dir);
	}
	bool			FileSystem::createDirectory(const std::string& s)
	{
		return createDirectory(FS::path(s));
	}
	bool			FileSystem::exists(const FS::path& p)
	{
		return FS::exists(p);
	}
	bool			FileSystem::exists(const std::string& p)
	{
		return FS::exists(FS::path(p));
	}
	bool			FileSystem::deleteFile(const FS::path& p)
	{
		if (!FileSystem::exists(p))
			return false;
		if (FS::is_directory(p))
			return FS::remove_all(p) > 0;
		return FS::remove(p);
	}
	bool			FileSystem::moveFile(const FS::path& src, const FS::path& dst)
	{
		return move(src, dst / src.filename());
	}
	bool			FileSystem::copyFile(const FS::path& src, const FS::path& dst)
	{
		return copy(src, dst / src.filename());
	}
	bool			FileSystem::isDirectory(const FS::path& p)
	{
		return FS::is_directory(p);
	}
				
	FileStatus		FileSystem::tryOpenFile(const FS::path& p)
	{
		HANDLE fH = CreateFile(p.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);

		if (fH == INVALID_HANDLE_VALUE)
		{
			DWORD err = GetLastError();
			if (err == ERROR_FILE_NOT_FOUND || err == ERROR_PATH_NOT_FOUND)
				return FileStatus::Invald;
			if (err == ERROR_SHARING_VIOLATION)
				return FileStatus::Locked;
			return FileStatus::Other;
		}
		CloseHandle(fH);
		return FileStatus::Success;
	}
				
	bool			FileSystem::isNewer(const FS::path& a, const FS::path& b)
	{
		return FS::last_write_time(a) > FS::last_write_time(b);
	}
				
	bool			FileSystem::move(const FS::path& src, const FS::path& dst)
	{
		if (FileSystem::exists(dst))
			return false;
		FS::rename(src, dst);
		return true;
	}
	bool			FileSystem::copy(const FS::path& src, const FS::path& dst)
	{
		if (FileSystem::exists(dst))
			return false;
		FS::copy(src, dst);
		return true;
	}
	bool			FileSystem::rename(const FS::path& src, const FS::path& dst)
	{
		return move(src, dst);
	}
	bool			FileSystem::renameFilename(const FS::path& old, const std::string name)
	{
		FS::path newPath = old.parent_path() / FS::path(name + old.extension().string());
		return rename(old, newPath);
	}
				
	FS::path		FileSystem::getUnqiueFileName(const FS::path& p)
	{
		if (!FileSystem::exists(p))
			return p;

		int c = 0;
		auto checkID = [&c, p](auto checkID) -> FS::path
		{
			++c;
			const std::string counterStr = [&c] {
				if (c < 10)
					return "0" + std::to_string(c);
				else
					return std::to_string(c);
			}();
			std::string newName = "{" + removeExtention(p.filename().string()) + "} ({" + counterStr + "})";
			if (p.has_extension())
				newName = "{" + newName + "}{" + p.extension().string() + "}";
			if (FS::exists(p.parent_path() / newName))
			{
				return checkID(checkID);
			}
			else
			{
				return p.parent_path() / newName;
			}
		};
		return checkID(checkID);
	}
	std::string FileSystem::removeExtention(const std::string& s)
	{
		return s.substr(0, s.find_last_of('.'));
	}
}