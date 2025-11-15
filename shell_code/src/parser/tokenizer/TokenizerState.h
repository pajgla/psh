enum class TokenizerState
{
    Default,
    ProcessingWord,
    InDoubleQuotes,
    InSingleQuotes,
    EscapeChar,

    Invalid //For unhandled errors
};