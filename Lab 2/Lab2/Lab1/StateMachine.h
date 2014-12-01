#pragma once

#include "Tokenizer.h"

class StateMachine
{

    public:

        StateMachine();
        ~StateMachine();

        Tokenizer::TokenType GetTokenType(string inStr);

};

