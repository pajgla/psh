#pragma once

class DebugHelper
{
public:
    static void PrintDebug(const char* c, ...);
    static void PrintError(const char* c, ...);

private:
    static void Print_Internal(const char* c, ...);
};