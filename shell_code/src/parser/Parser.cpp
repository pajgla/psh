#include "./Parser.h"

#include "../debug/DebugHelper.h"

bool Parser::PeekTokenAtCurrentPosition(Token& outToken) const
{
    if (m_CurrTokenPos >= m_Tokens.size())
    {
        return false;
    }

    outToken = m_Tokens[m_CurrTokenPos];
    return true;
}

bool Parser::ConsumeTokenAtCurrentPosition(Token& outToken)
{
    if (m_CurrTokenPos >= m_Tokens.size())
    {
        return false;
    }

    outToken = m_Tokens[m_CurrTokenPos];
    m_CurrTokenPos++;
    return true;
}

bool Parser::CheckTokenTypeAtCurrentPosition(const TokenType type) const
{
    if (m_CurrTokenPos >= m_Tokens.size())
    {
        return false;
    }

    return m_Tokens[m_CurrTokenPos].m_TokenType == type;
}

// void PrintTree(std::unique_ptr<ParserNode> root)
// {
//     if (root == nullptr)
//         return;

//     PrintTree(std::move(root->m_LeftNode));
//     std::string nodeName = root->m_ParserNodeType == ParserNodeType::Command ? "Command" : "Sequence";
//     DebugHelper::PrintDebug("Parser Node: %s , holding values:", nodeName.c_str());

//     for (auto arg : root->m_Args)
//     {
//         DebugHelper::PrintDebug("Arg: %s", arg.c_str());
//     }

//     PrintTree(std::move(root->m_RightNode));

// }

std::unique_ptr<ParserNode> Parser::Parse()
{
    std::unique_ptr<ParserNode> node = ParseSequence();
    if (node == nullptr)
    {
        DebugHelper::PrintError("Parser returned nullptr node");
        return nullptr;
    }

    return node;
    //PrintTree(std::move(node));
}

std::unique_ptr<ParserNode> Parser::ParseCommand()
{
    std::vector<std::string> cmdArgs;

    while (CheckTokenTypeAtCurrentPosition(TokenType::Word))
    {
        Token outToken;
        if (!ConsumeTokenAtCurrentPosition(outToken))
        {
            DebugHelper::PrintDebug("Something went wrong while parsing tokens...");
            return nullptr;
        }

        if (outToken.m_Text.empty())
        {
            DebugHelper::PrintDebug("One of the tokens has empty arg value, which is not allowed. Aborting...");
            return nullptr;
        }

        cmdArgs.push_back(outToken.m_Text);
    }

    if (cmdArgs.empty())
    {
        DebugHelper::PrintError("Expected a command but found none while parsing");
        return nullptr;
    }

    return std::make_unique<ParserNode>(cmdArgs);
}

std::unique_ptr<ParserNode> Parser::ParseSequence()
{
    std::unique_ptr<ParserNode> left = ParseCommand();

    while (CheckTokenTypeAtCurrentPosition(TokenType::Semicolon))
    {
        Token outToken;
        if (!ConsumeTokenAtCurrentPosition(outToken))
        {
            DebugHelper::PrintError("Failed to consume operator token");
            return nullptr;
        }

        auto right = ParseCommand();
        auto newSequence = std::make_unique<ParserNode>(ParserNodeType::Sequence, std::move(left), std::move(right));
        left = std::move(newSequence);
    }

    return left;
}