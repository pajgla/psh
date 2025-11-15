#include "CommandHolder.h"

#include "InternalCommand.h"
#include "ChangeDirectoryInternalCommand.h"
#include "../debug/DebugHelper.h"

void CommandHolder::Init()
{
    RegisterCmd<ChangeDirectoryInternalCommand>();
}

const InternalCommand* CommandHolder::FindInternalCommand(const char* cmdName)
{
    if (cmdName == nullptr)
    {
        return nullptr;
    }

    for (auto& [cmdCallName, cmdObj] : m_CmdMap)
    {
        if (cmdCallName == cmdName)
        {
            return cmdObj;
        }
    }

    return nullptr;
}