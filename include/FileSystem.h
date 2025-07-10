#pragma once

#include "Exports.h"

namespace che
{
	class CORE_API FileSystem
	{
	public:
		static const std::string& getAppDir();
		static void setAppDir(const std::string_view&);

	private:
		static std::string g_appDir;
	};
}
