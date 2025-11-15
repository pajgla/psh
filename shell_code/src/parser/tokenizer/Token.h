#pragma once

#include <string>
#include "./TokenType.h"

struct Token
{
    TokenType m_TokenType;
    std::string m_Text;
};