#include "DebugHelper.h"

#include <cstdio>

static constexpr bool ENABLED = false;

void DebugHelper::PrintDebug(const char* format, ...)
{
    DebugHelper::Print_Internal(format);
}

void DebugHelper::PrintError(const char* format, ...)
{
    DebugHelper::Print_Internal(format);
}

void DebugHelper::Print_Internal(const char* format, ...)
{
    if (ENABLED)
    {
        printf(format);
        printf("\n");
    }
}