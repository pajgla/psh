#include "ChangeDirectoryInternalCommand.h"

#include <cstring>
#include <unistd.h>
#include "../debug/DebugHelper.h"

bool ChangeDirectoryInternalCommand::Execute(const std::vector<char*>& args) const
{
    DebugHelper::PrintDebug("Executing 'cd' command...");

    if (args.size() > 2)
    {
        perror("'cd' command expect 0 or 1 argument");
        return false;
    }

    const char* dir = "";
    if (args.size() > 1 && args[1] != nullptr)
    {
        dir = args[1];
        DebugHelper::PrintDebug("Requested directory '%s'", dir);
    }

    if (strcmp(dir, "") == 0 || strcmp(dir, "~") == 0)
    {
        const char* homeDir = getenv("HOME");
        if (homeDir)
        {
            dir = homeDir;
        }
    }

    chdir(dir);

    return true;
}