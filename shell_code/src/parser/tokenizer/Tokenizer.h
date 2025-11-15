#pragma once

#include <string>
#include <vector>
#include "./Token.h"
#include "./TokenizerState.h"

class Tokenizer
{
public:
    Tokenizer(const std::string& input): m_Input(input), m_CharPos(0)
    {

    }

    std::vector<Token> Tokenize();

private:
    //States
    void ProcessDefaultState();
    void ProcessWordState();
    void ProcessDoubleQuotesState();
    void ProcessSingleQuotesState();
    void ProcessEscapeChar();

    bool CheckState(TokenizerState expectedState, const char* stateName);
    void ChangeState(TokenizerState newState, bool advancePos = true);

    std::string m_Input;
    int m_CharPos = 0;
    std::string m_CurrentWord;
    std::vector<Token> m_Tokens;
    TokenizerState m_State = TokenizerState::Default;
    TokenizerState m_PreviousState = TokenizerState::Default;
};