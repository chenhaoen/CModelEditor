#include "CFileSystem.h"

std::string CFileSystem::g_appDir;

const std::string& CFileSystem::getAppDir()
{
    return g_appDir;
}

void CFileSystem::setAppDir(const std::string_view& view)
{
    std::filesystem::path path(view);
    g_appDir = std::filesystem::canonical(path.parent_path()).string();
}
