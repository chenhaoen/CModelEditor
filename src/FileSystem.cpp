#include "FileSystem.h"

using namespace che;

std::string FileSystem::g_appDir;

const std::string& FileSystem::getAppDir()
{
    return g_appDir;
}

void FileSystem::setAppDir(const std::string_view& view)
{
    std::filesystem::path path(view);
    g_appDir = std::filesystem::canonical(path.parent_path()).string();
}
