#include "./TokenizerHelpers.h"
#include "../../../debug/DebugHelper.h"

namespace TokenizerHelpers
{
    void PrintTokens(const std::vector<Token>& tokens)
    {
        for (const Token& token : tokens)
        {
            const std::string tokenName = TokenTypeToString(token.m_TokenType);
            DebugHelper::PrintDebug("[(Type:%s):(Word:%s)] ", tokenName.c_str(), token.m_Text.c_str());
        }
    }

    std::string TokenTypeToString(const TokenType tokenType)
    {
        switch (tokenType)
        {
        case TokenType::Word:
            return "Word";

        case TokenType::Semicolon:
            return "Semicolon";
        case TokenType::EndOfFile:
            return "EndOfFile";
        
        default:
            DebugHelper::PrintError("Unhandled TokenType in TokenizerHelpers::TokenTypeToString");
            return "UnhandledType";
        }
    }
}