#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>

#include "debug/DebugHelper.h"
#include "internalcommands/CommandHolder.h"
#include "parser/tokenizer/Tokenizer.h"
#include "parser/tokenizer/helpers/TokenizerHelpers.h"

int ProcessKernelCmd(std::vector<char*> arguments);
bool Init();

//Dependencies
CommandHolder cmdHolder;

int main() {
    std::string testLine;
    while (true)
    {
        std::getline(std::cin, testLine);
        if (testLine.empty())
        {
            continue;
        }

        Tokenizer tokenizer(testLine);
        std::vector<Token> tokens = tokenizer.Tokenize();
        TokenizerHelpers::PrintTokens(tokens);
        return 0;
    }
    
    if (!Init())
    {
        DebugHelper::PrintError("An error occured. Closing psh...");
        return -1;
    }

    std::string line;

    while (true) {

        std::cout << "psh$ ";

        if (!std::getline(std::cin, line)) {
            break;
        }

        if (line.empty()) {
            continue;
        }

        std::stringstream stringStream(line);
        std::string token;
        std::vector<char*> arguments;

        while (stringStream >> token) {
            const char* ptrToken = token.c_str();
            char* stringCopy = strdup(ptrToken);
            arguments.push_back(stringCopy);
        }

        if (arguments.empty()) {
            continue;
        }

        if (const InternalCommand* internalCmd = cmdHolder.FindInternalCommand(arguments[0]))
        {
            DebugHelper::PrintDebug("Processing internal cmd...");
            internalCmd->Execute(arguments);
        }
        else
        {
            DebugHelper::PrintDebug("Processing kernel cmd...");
            const int processReturn = ProcessKernelCmd(arguments);
            if (processReturn != 0)
            {
                return processReturn;
            }
        }
    }

    //Psh completed
    return 0;
}

// void ParseUserInput(const std::string userInput, std::vector<std::string>& outCmds)
// {
//     std::string line;
//     for (const char c : userInput)
//     {
//         if (arg == nullptr)
//         {
//             continue;
//         }

//         if (strcmp(arg, ";") == 0)
//         {
//             outCmds.push_back(line);
//             line = "";
//         }
//         else
//         {
//             line
//         }
//     }
// }

int ProcessKernelCmd(std::vector<char*> arguments)
{
    //execvp expects nullptr at the end of the arg list
    arguments.push_back(nullptr);

    const pid_t pid = fork();

    if (pid < 0) {
        perror("fork error");
        return 1;
    }

    if (pid == 0) {
        // Child process only
        execvp(arguments[0], arguments.data());

        //Only reached on error since execvp swaps processes
        perror("execvp");
        return 1;
    }

    // Parent waits
    int status;
    waitpid(pid, &status, 0);
    
    // Free allocated strings
    for (char* a : arguments) {
        free(a);
    }

    return 0;
}

bool Init()
{
    cmdHolder.Init();

    return true;
}