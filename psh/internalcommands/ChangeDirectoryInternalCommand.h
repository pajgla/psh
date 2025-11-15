#pragma once

#include "InternalCommand.h"

class ChangeDirectoryInternalCommand : public InternalCommand
{
public:
    ChangeDirectoryInternalCommand(): InternalCommand("cd") {}

    virtual bool Execute(const std::vector<char*>& args) const override;
};