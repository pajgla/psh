#pragma once

#include <vector>
#include <memory>
#include "./tokenizer/Token.h"
#include "./ParserNode.h"

class Parser
{
public:
    Parser(const std::vector<Token>& tokens): m_Tokens(tokens) {}

    //Returns a root of our tree
    std::unique_ptr<ParserNode> Parse();

private:
    bool PeekTokenAtCurrentPosition(Token& outToken) const;
    bool ConsumeTokenAtCurrentPosition(Token& outToken);
    bool CheckTokenTypeAtCurrentPosition(const TokenType type) const;

    //Parsing
    std::unique_ptr<ParserNode> ParseCommand();
    std::unique_ptr<ParserNode> ParseSequence();

    std::unique_ptr<ParserNode> m_RootNode = nullptr;
    std::vector<Token> m_Tokens;
    int m_CurrTokenPos = 0;
};