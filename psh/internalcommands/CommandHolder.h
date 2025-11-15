#pragma once

#include <unordered_map>
#include <string>
#include "./InternalCommand.h"
#include "../debug/DebugHelper.h"


class CommandHolder
{
public:
    void Init();

    const InternalCommand* FindInternalCommand(const char* cmdName);

private:
    template<typename T>
    void RegisterCmd()
    {
        static_assert(std::is_base_of<InternalCommand, T>::value, "Error: T must inherit from InternalCommand class");

        T* newCmdObj = new T();
        const char* cmdCallName = newCmdObj->GetCommandCallName();
        m_CmdMap[cmdCallName] = newCmdObj;

        DebugHelper::PrintDebug("Registering internal cmd '%s'",cmdCallName);
    }

    std::unordered_map<std::string, InternalCommand*> m_CmdMap;
};