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
#include "parser/Parser.h"

int ProcessKernelCmd(std::vector<char*> arguments);
bool Init();
void ProcessParserTree(std::unique_ptr<ParserNode> root);

//Dependencies
CommandHolder cmdHolder;

int main() {    
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

        //Tokenize
        Tokenizer tokenizer(line);
        std::vector<Token> tokens = tokenizer.Tokenize();

        //Parse
        Parser parser(tokens);
        auto rootNode = parser.Parse();
        ProcessParserTree(std::move(rootNode));
    }

    //Psh completed
    return 0;
}

void ProcessParserTree(std::unique_ptr<ParserNode> root)
{
    if (root == nullptr)
        return;

    if (root->m_ParserNodeType == ParserNodeType::Command)
    {
        if (root->m_Args.size() == 0)
        {
            DebugHelper::PrintError("Parser command node has empty args");
            return;
        }

        const std::string& cmd = root->m_Args[0];
        std::vector<char*> cArgs;
        cArgs.reserve(root->m_Args.size());
        for (auto& s : root->m_Args)
        {
            cArgs.push_back(s.data());
        }

        if (const InternalCommand* internalCmd = cmdHolder.FindInternalCommand(cmd.c_str()))
        {
            DebugHelper::PrintDebug("Processing internal cmd...");
            internalCmd->Execute(cArgs);
        }
        else
        {
            DebugHelper::PrintDebug("Processing kernel cmd...");
            const int processReturn = ProcessKernelCmd(cArgs);
            if (processReturn != 0)
            {
                DebugHelper::PrintError("Kernel process returned error");
            }
        }
    }
    else if (root->m_ParserNodeType == ParserNodeType::Sequence)
    {
        ProcessParserTree(std::move(root->m_LeftNode));
        ProcessParserTree(std::move(root->m_RightNode));
    }
}

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
    // for (char* a : arguments) {
    //     free(a);
    // }

    return 0;
}

bool Init()
{
    cmdHolder.Init();

    return true;
}