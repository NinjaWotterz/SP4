/**
 FileSystem
 Date: Mar 2020
 */
#pragma once

#include <string>
#include <cstdlib>
#include "../GameControl/Settings.h"

#ifdef __linux__ 
	#include <unistd.h>
	#define GetCurrentDir getcwd
#elif _WIN32
	#include <direct.h>
	#define GetCurrentDir _getcwd
#endif

class FileSystem
{
private:
	typedef std::string(*Builder) (const std::string& path);

public:
	static std::string getPath(const std::string& path)
	{
		static std::string(*pathBuilder)(std::string const &) = getPathBuilder();
		return (*pathBuilder)(path);
	}

private:
	static std::string const & getRoot()
	{
		char buff[FILENAME_MAX]; //create string buffer to hold path
		GetCurrentDir(buff, FILENAME_MAX);
		string current_working_dir(buff);

		static char const * envRoot = current_working_dir.c_str();	// getenv("LOGL_ROOT_PATH");
		static char const * givenRoot = (envRoot != nullptr ? envRoot : CSettings::GetInstance()->logl_root);
			//logl_root);
		static std::string root = (givenRoot != nullptr ? givenRoot : "");
		return root;
	}

	//static std::string(*foo (std::string const &)) getPathBuilder()
	static Builder getPathBuilder()
	{
		if (getRoot() != "")
			return &FileSystem::getPathRelativeRoot;
		else
			return &FileSystem::getPathRelativeBinary;
	}

	static std::string getPathRelativeRoot(const std::string& path)
	{
		return getRoot() + std::string("/") + path;
	}

	static std::string getPathRelativeBinary(const std::string& path)
	{
		return "../../../" + path;
	}
};
