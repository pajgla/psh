#pragma once

#include <vector>
#include <string>
#include "../Token.h"

namespace TokenizerHelpers
{
    void PrintTokens(const std::vector<Token>& tokens);
    std::string TokenTypeToString(const TokenType tokenType);
}