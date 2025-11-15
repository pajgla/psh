#include "./Tokenizer.h"
#include "../../debug/DebugHelper.h"

std::vector<Token> Tokenizer::Tokenize()
{
    m_State = TokenizerState::Default;
    std::string currentWord = "";

    while (m_CharPos < m_Input.length())
    {
        switch (m_State)
        {
            case TokenizerState::Default:
                ProcessDefaultState();
                break;
            case TokenizerState::ProcessingWord:
                ProcessWordState();
                break;
            case TokenizerState::InDoubleQuotes:
                ProcessDoubleQuotesState();
                break;
            case TokenizerState::InSingleQuotes:
                ProcessSingleQuotesState();
                break;
            case TokenizerState::EscapeChar:
                ProcessEscapeChar();
                break;

            //An error occured
            case TokenizerState::Invalid:
                DebugHelper::PrintError("An error occured while tokenizing the input. Stopping the tokenization process...");
                return std::vector<Token>();

            default:
                DebugHelper::PrintError("Unhandled state in Tokenizer");
                return std::vector<Token>();
        }
    }

    //Handle leftover words
    if (!m_CurrentWord.empty())
    {
        m_Tokens.push_back(Token{TokenType::Word, m_CurrentWord});
    }

    m_Tokens.push_back(Token{TokenType::EndOfFile});
    return m_Tokens;

    return m_Tokens;
}

bool Tokenizer::CheckState(TokenizerState expectedState, const char* stateName)
{
    if (m_State != expectedState)
    {
        DebugHelper::PrintError("Tried processing %s State but the Tokenizer is in different state currently. Aborting tokenization...", stateName);
        m_State = TokenizerState::Invalid;
        return false;
    }

    return true;
}

void Tokenizer::ChangeState(TokenizerState newState, bool advancePos /* = true*/)
{
    m_PreviousState = m_State;
    m_State = newState;
    if (advancePos)
    {
        m_CharPos++;
    }
}

void Tokenizer::ProcessDefaultState()
{
    if (!CheckState(TokenizerState::Default, "Default"))
    {
        return;
    }

    const char& c = m_Input[m_CharPos];

    if (isspace(c))
    {
        m_CharPos++;
    }
    else if (c == ';')
    {
        m_Tokens.push_back(Token{TokenType::Semicolon, ";"});
        m_CharPos++;
    }
    else if (c == '"')
    {
        ChangeState(TokenizerState::InDoubleQuotes);
    }
    else if (c == '\'')
    {
        ChangeState(TokenizerState::InSingleQuotes);
    }
    else if (c == '\\')
    {
        ChangeState(TokenizerState::EscapeChar);
    }
    else
    {
        //We started reading a word
        m_CurrentWord = c;
        ChangeState(TokenizerState::ProcessingWord);
    }
}

void Tokenizer::ProcessWordState()
{
    if (!CheckState(TokenizerState::ProcessingWord, "ProcessingWord"))
    {
        return;
    }

    const char& c = m_Input[m_CharPos];
    if (isspace(c) || c == ';')
    {
        m_Tokens.push_back(Token{TokenType::Word, m_CurrentWord});
        m_CurrentWord.clear();
        //We do not increase the char pos since we want to let the default state to process it
        ChangeState(TokenizerState::Default, false);
    }
    else if (c == '"')
    {
        ChangeState(TokenizerState::InDoubleQuotes);
    }
    else if (c == '\'')
    {
        ChangeState(TokenizerState::InSingleQuotes);
    }
    else if (c == '\\')
    {
        ChangeState(TokenizerState::EscapeChar);
    }
    else 
    {
        m_CurrentWord += c;
        m_CharPos++;
    }
}

void Tokenizer::ProcessDoubleQuotesState()
{
    if (!CheckState(TokenizerState::InDoubleQuotes, "InDoubleQuotes"))
    {
        return;
    }

    const char& c = m_Input[m_CharPos];
    if (c == '"')
    {
        //Return to the word processing state so we can finalize the whole word we processed in dquotes
        ChangeState(TokenizerState::ProcessingWord);
    }
    else if (c == '\\')
    {
        if (m_CharPos + 1 < m_Input.length())
        {
            const char& next = m_Input[m_CharPos + 1];
            //#TODO: Add table of allowed escape characters
            if (next == '"' || next == '\\' || next == '$')
            {
                ChangeState(TokenizerState::EscapeChar);
            }
            else
            {
                //Not counted as a special char
                m_CurrentWord += c;
                m_CharPos++;
            }
        }
        else
        {
            m_CurrentWord += c;
            m_CharPos++;
        }
    }
    else
    {
        m_CurrentWord += c;
        m_CharPos++;
    }
}

void Tokenizer::ProcessSingleQuotesState()
{
    if (!CheckState(TokenizerState::InSingleQuotes, "InSingleQuotes"))
    {
        return;
    }

    const char& c = m_Input[m_CharPos];
    if (c == '\'')
    {
        ChangeState(TokenizerState::ProcessingWord);
    }
    else
    {
        m_CurrentWord += c;
        m_CharPos++;
    }
}

void Tokenizer::ProcessEscapeChar()
{
    const char& c = m_Input[m_CharPos];
    m_CurrentWord += c;

    if (m_PreviousState == TokenizerState::Default)
    {
        ChangeState(TokenizerState::ProcessingWord);
    }
    else
    {
        ChangeState(m_PreviousState);
    }
}