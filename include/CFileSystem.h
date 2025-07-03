#pragma once

#include "Exports.h"

class CORE_API CFileSystem
{
public:
	static const std::string& getAppDir();
	static void setAppDir(const std::string_view&);

private:
	static std::string g_appDir;
};

