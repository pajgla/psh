#pragma once

#include <vector>
#include <string>

class InternalCommand
{
public:
    InternalCommand(const char* cmdName) : m_CommandCallName(cmdName)
    {

    }

    virtual bool Execute(const std::vector<char*>& args) const = 0;

    const char* GetCommandCallName() const { return m_CommandCallName; }
protected:
    const char* m_CommandCallName = "Missing";
};